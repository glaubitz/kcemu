/*
 *  KCemu -- the KC 85/3 and KC 85/4 Emulator
 *  Copyright (C) 1997-2001 Torsten Paul
 *
 *  $Id: about.h,v 1.10 2002/10/31 01:38:07 torsten_paul Exp $
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

#ifndef __ui_gtk_about_h
#define __ui_gtk_about_h

#include <gtk/gtk.h>

#include "kc/system.h"

#include "ui/gtk/window.h"

class AboutWindow : public UI_Gtk_Window
{
 private:
  struct {
    GtkWidget *vbox;
    GtkWidget *name;
    GtkWidget *copyright;
    GtkWidget *info;
    GtkWidget *separator;
    GtkWidget *close;
  } _w;

  static char APP_NAME[];
  static char APP_COPYRIGHT[];

  CMD *_cmd;

 protected:
  void init(void);
  
 public:
  AboutWindow(void);
  virtual ~AboutWindow(void);
};

#endif /* __ui_gtk_about_h */
