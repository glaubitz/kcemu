/*
 *  KCemu -- the KC 85/3 and KC 85/4 Emulator
 *  Copyright (C) 1997-2001 Torsten Paul
 *
 *  $Id: load_UNKNOWN.c,v 1.7 2002/10/31 00:48:52 torsten_paul Exp $
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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileio/loadP.h"

static int
loader_UNKNOWN_check(const char *filename,
                     unsigned char *data,
                     long size)
{
  return 1;
}

static int
str2int(const char *str)
{
  int a;
  unsigned long value;
  char *endptr, buf[11];

  strncpy(buf, str, 10);
  buf[10] = '\0';
  a = 2;
  while ((buf[a] != '\0') && (strchr("0123456789abcdefABCDEF", buf[a]) != NULL))
    a++;
  buf[a] = '\0';
  if (a < 3)
    return -1;
  value = strtoul(buf, &endptr, 0);
  if (*endptr == '\0')
    if ((value >= 0) && (value <= 0xffff))
      return value;
  return -1;
}

static int
loader_UNKNOWN_load(const char *filename,
                    unsigned char *data,
                    long size,
                    fileio_prop_t **prop)
{
  int a;
  const char *ptr;
  unsigned long val, xsize;
  unsigned short load, start;

  xsize = size + (size + 127) / 128;

  a = 0;
  load = 0;
  start = 0;
  ptr = strstr(filename, "0x");
  if (ptr != NULL)
    {
      val = str2int(ptr);
      if (val != -1)
        {
          load = val;
          a = 1;
        }
    }
  if (a == 1)
    {
      ptr = strstr(ptr + 1, "0x");
      if (ptr != NULL)
        {
          val = str2int(ptr);
          if (val != -1)
            {
              start = val;
              a = 2;
            }
        }
    }

  *prop = (fileio_prop_t *)malloc(sizeof(fileio_prop_t));
  if (*prop == NULL)
    return -1;
  memset(*prop, 0, sizeof(fileio_prop_t));

  (*prop)->type = FILEIO_TYPE_UNKNOWN;
  (*prop)->valid = FILEIO_V_NONE;
  (*prop)->load_addr  = 0;
  (*prop)->start_addr = 0xffff;
  switch (a)
    {
    case 2:
      (*prop)->start_addr = start;
      (*prop)->valid |= FILEIO_V_AUTOSTART;
      (*prop)->valid |= FILEIO_V_START_ADDR;
  /* fall through */
    case 1:
      (*prop)->load_addr  = load;
      (*prop)->valid |= FILEIO_V_LOAD_ADDR;
      (*prop)->type = FILEIO_TYPE_COM;
      break;
    }
  (*prop)->autostart = (a == 2);

  /*
   *  guess a name for the kc file header and make it all upper case
   */
  ptr = strrchr(filename, '/');
  if (ptr)
    ptr++;
  else
    ptr = filename;
  memcpy((*prop)->name, ptr, 11);
  (*prop)->name[11] = '\0';

  for (a = 0;a < 11;a++)
    (*prop)->name[a] = toupper((*prop)->name[a]);

  /*
   *  copy data...
   */
  (*prop)->size  = size + 128;
  (*prop)->data = (unsigned char *)malloc(xsize + 129);
  if ((*prop)->data == 0)
    return -1;

  *(*prop)->data = 0; // block number
  fill_header_COM((*prop)->data + 1, *prop);
  fileio_copy_blocks((*prop)->data + 129, data, size, 1);

  return 0;
}

static const char *
loader_UNKNOWN_get_type(void)
{
  return "UNKNOWN";
}

static const char *
loader_UNKNOWN_get_name(void)
{
  return "fallback loader for unknown file types";
}

static file_loader_t loader = {
  loader_UNKNOWN_check,
  loader_UNKNOWN_load,
  loader_UNKNOWN_get_type,
  loader_UNKNOWN_get_name
};

void loader_UNKNOWN_init(void)
{
  fileio_register_loader(&loader);
}
