/*
 *  KCemu -- the KC 85/3 and KC 85/4 Emulator
 *  Copyright (C) 1997-2001 Torsten Paul
 *
 *  $Id: kcemu-remote.c,v 1.3 2002/10/31 01:38:12 torsten_paul Exp $
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

#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/WinUtil.h>

void
send(Display *disp, const char *cmd, int len)
{
  Atom atom, type_ret;
  int a, format_ret;
  unsigned char *prop_ret;
  unsigned int nchildren_ret;
  unsigned long nitems_ret, bytes_after_ret;
  Window w, root, root_ret, parent_ret, *children_ret;

  atom = XInternAtom(disp, "_KCEMU_REMOTE_COMMAND", False);
  root = DefaultRootWindow(disp);
  
  if (!XQueryTree(disp, root,
                  &root_ret, &parent_ret,
                  &children_ret, &nchildren_ret))
    return;

  for (a = 0;a < nchildren_ret;a++)
    {
      w = XmuClientWindow(disp, children_ret[a]);
      XGetWindowProperty(disp, w,
                         atom, 0, (65536 / sizeof(long)), False,
                         XA_STRING, &type_ret, &format_ret,
                         &nitems_ret, &bytes_after_ret,
                         &prop_ret);
      if (type_ret == None)
        continue;
      printf("property found! on window %x -> %s\n", (unsigned int)w, prop_ret);
      XChangeProperty(disp, w,
                      atom, XA_STRING, 8, PropModeReplace,
                      cmd, len);
    }
}

const char *
pack_args(int argc, char **argv, int *len_ret)
{
	int a, len;
	char *args, *ptr;
	
	printf("argc = %d\n", argc);

	len = 0;
	for (a = 1;a < argc;a++) {
		printf("argv[%d] = %s\n", a, argv[a]);
		len += strlen(argv[a]) + 1;
	}
	
	if (!(args = (char *)malloc(len)))
		return 0;

	ptr = args;
	for (a = 1;a < argc;a++) {
		strcpy(ptr, argv[a]);
		ptr += strlen(argv[a]) + 1;
	}
	
	*len_ret = len;
	return args;
}

int
main(int argc, char **argv)
{
  Display *disp;
  int len;
  const char *args;
  
  if (argc < 2)
	  return 1;

  disp = XOpenDisplay("");
  if (disp == 0)
    {
      printf("can't open display\n");
      exit(1);
    }
  
  args = pack_args(argc, argv, &len);
  if (!args)
	  return 1;

  send(disp, args, len);
  return 0;
}
