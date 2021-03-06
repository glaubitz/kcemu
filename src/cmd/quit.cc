/*
 *  KCemu -- The emulator for the KC85 homecomputer series and much more.
 *  Copyright (C) 1997-2010 Torsten Paul
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
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "kc/kc.h"
#include "kc/z80.h"
#include "ui/status.h"
#include "cmd/cmd.h"

class CMD_quit : public CMD
{
public:
  
  CMD_quit(void) : CMD("Quit")
    {
      register_cmd("emu-quit");
    }

  void execute(CMD_Args *args, CMD_Context context)
    {
      Status::instance()->setMessage("*** QUIT ***");
      CMD_Repository::instance()->dump();
      z80->quit();
    }
};

__force_link(CMD_quit);
