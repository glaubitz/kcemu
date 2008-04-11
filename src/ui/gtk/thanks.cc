/*
 *  KCemu -- the KC 85/3 and KC 85/4 Emulator
 *  Copyright (C) 1997-2006 Torsten Paul
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

#include "kc/system.h"

#include "ui/gtk/cmd.h"
#include "ui/gtk/thanks.h"

class CMD_thanks_window_toggle : public CMD
{
private:
  ThanksWindow *_w;

public:
  CMD_thanks_window_toggle(ThanksWindow *w) : CMD("ui-thanks-window-toggle")
    {
      _w = w;
      register_cmd("ui-thanks-window-toggle");
    }

  void execute(CMD_Args *args, CMD_Context context)
    {
      _w->toggle();
    }
};

ThanksWindow::ThanksWindow(const char *glade_xml_file) : UI_Gtk_Window(glade_xml_file)
{
  _cmd = new CMD_thanks_window_toggle(this);
}

ThanksWindow::~ThanksWindow(void)
{
  delete _cmd;
}

void
ThanksWindow::init(void)
{
  _window = get_widget("thanks_window");
  gtk_signal_connect(GTK_OBJECT(_window), "delete_event",
		     GTK_SIGNAL_FUNC(cmd_exec_sft),
		     (char *)"ui-thanks-window-toggle"); // FIXME:

  init_dialog("ui-thanks-window-toggle", NULL);
}
