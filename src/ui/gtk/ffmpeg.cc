/*
 *  KCemu -- the KC 85/3 and KC 85/4 Emulator
 *  Copyright (C) 1997-2009 Torsten Paul
 *
 *  $Id$
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "kc/config.h"
#ifdef HAVE_LIBAVFORMAT

#include "kc/system.h"

#include "ui/gtk/ffmpeg.h"

FfmpegVideoEncoder::FfmpegVideoEncoder(void)
{
}

FfmpegVideoEncoder::~FfmpegVideoEncoder(void)
{
  close();
}

bool
FfmpegVideoEncoder::init(const char *filename, int width, int height, double quality)
{
  if (filename == NULL)
    return false;

  _width = width;
  _height = height;

  av_register_all();

  AVOutputFormat *fmt = guess_format("avi", NULL, "video/x-msvideo");
  if (fmt == NULL)
    return false;

  _context = avformat_alloc_context();
  _context->oformat = fmt;
  snprintf(_context->filename, sizeof (_context->filename), "%s", filename);

  _stream = av_new_stream(_context, 0);
  _stream->codec->codec_id = fmt->video_codec;
  _stream->codec->codec_type = CODEC_TYPE_VIDEO;
  _stream->codec->codec_tag = MKTAG('D', 'X', '5', '0');

  _stream->codec->bit_rate = 79000 + 1000 * pow(1.4, quality * 20.0);
  _stream->codec->width = width;
  _stream->codec->height = height;
  _stream->codec->time_base.den = 50;
  _stream->codec->time_base.num = 1;
  _stream->codec->gop_size = 25;
  _stream->codec->pix_fmt = PIX_FMT_YUV420P;

  // some formats want stream headers to be separate
  if (_context->oformat->flags & AVFMT_GLOBALHEADER)
    _stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;

  if (av_set_parameters(_context, NULL) < 0)
    return false;

  AVCodec *codec = avcodec_find_encoder(_stream->codec->codec_id);
  if (avcodec_open(_stream->codec, codec) < 0)
    return false;

  _bufsize = 10 * width * height;
  _buf = (byte_t *)av_malloc(_bufsize);

  _frame = avcodec_alloc_frame();
  if (_frame == NULL)
    return false;

  int size = avpicture_get_size(_stream->codec->pix_fmt, width, height);
  byte_t *buf = (byte_t *) av_malloc(size);
  if (buf == NULL)
    {
      av_free(_frame);
      return false;
    }

  avpicture_fill((AVPicture *) _frame, buf, _stream->codec->pix_fmt, width, height);

  if (url_fopen(&_context->pb, filename, URL_WRONLY) < 0)
    return false;

  //dump_format(_context, 0, filename, 1);
  av_write_header(_context);
  return true;
}

void
FfmpegVideoEncoder::allocate_color_rgb(int idx, int r, int g, int b)
{
  _col[idx].y =  (0.257 * r) + (0.504 * g) + (0.098 * b) + 16;
  _col[idx].u = -(0.148 * r) - (0.291 * g) + (0.439 * b) + 128;
  _col[idx].v =  (0.439 * r) - (0.368 * g) - (0.071 * b) + 128;
}

void
FfmpegVideoEncoder::allocate_color_hsv(int idx, double h, double s, double v)
{
  int r, g, b;

  hsv2rgb(h, s, v, &r, &g, &b);
  allocate_color_rgb(idx, r, g, b);
}

bool
FfmpegVideoEncoder::encode(byte_t *image, byte_t *dirty)
{
  for (int y = 0;y < _height;y++)
    {
      int z = y * _frame->linesize[0];
      for (int x = 0;x < _width;x++)
        {
          _frame->data[0][z + x] = _col[image[z + x]].y;
        }
    }
  for (int y = 0;y < _height / 2;y++)
    {
      int z = 2 * y * _width;
      for (int x = 0;x < _width;x += 2)
        {
          int u = _col[image[z + x]].u + _col[image[z + x + 1]].u + _col[image[z + x + _width]].u + _col[image[z + x + _width + 1]].u;
          int v = _col[image[z + x]].v + _col[image[z + x + 1]].v + _col[image[z + x + _width]].v + _col[image[z + x + _width + 1]].v;
          _frame->data[1][y * _frame->linesize[1] + x / 2] = u / 4;
          _frame->data[2][y * _frame->linesize[2] + x / 2] = v / 4;
        }
    }

  int out_size = avcodec_encode_video(_stream->codec, _buf, _bufsize, _frame);
  if (out_size == 0)
    return true;

  AVPacket pkt;
  av_init_packet(&pkt);

  if (_stream->codec->coded_frame->pts != AV_NOPTS_VALUE)
    pkt.pts = av_rescale_q(_stream->codec->coded_frame->pts, _stream->codec->time_base, _stream->time_base);
  if (_stream->codec->coded_frame->key_frame)
    pkt.flags |= PKT_FLAG_KEY;
  pkt.stream_index = _stream->index;
  pkt.data = _buf;
  pkt.size = out_size;

  return av_interleaved_write_frame(_context, &pkt) == 0;
}

void
FfmpegVideoEncoder::close(void)
{
  av_write_trailer(_context);
  avcodec_close(_stream->codec);
  av_free(_frame->data[0]);
  av_free(_frame);
  av_free(_buf);

  /* free the streams */
  for (int i = 0;i < _context->nb_streams;i++)
    {
      av_freep(&_context->streams[i]->codec);
      av_freep(&_context->streams[i]);
    }
  url_fclose(_context->pb);
  
  /* free the stream */
  av_free(_context);
}

#endif /* HAVE_LIBAVFORMAT */