/*
 *  KCemu -- the KC 85/3 and KC 85/4 Emulator
 *  Copyright (C) 1997-2001 Torsten Paul
 *
 *  $Id: reset.cc,v 1.3 2002/01/06 12:53:40 torsten_paul Exp $
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

#include "kc/kc.h"
#include "kc/z80.h"
#include "ui/status.h"
#include "cmd/cmd.h"

class CMD_reset : public CMD
{
public:
  
  CMD_reset(void) : CMD("Reset")
    {
      register_cmd("emu-reset", 0);
      register_cmd("emu-power-on", 1);
    }

  void execute(CMD_Args *args, CMD_Context context)
    {
      switch (context)
        {
        case 0:
          Status::instance()->setMessage("*** RESET ***");
	  switch (get_kc_type())
	    {
	    case KC_TYPE_85_1:
	    case KC_TYPE_87:
	      z80->reset(0xf000);
	      break;
	      
	    case KC_TYPE_85_2:
	    case KC_TYPE_85_3:
	    case KC_TYPE_85_4:
	      z80->reset(0xe000);
	      break;
	    }
          break;
        case 1:
          Status::instance()->setMessage("*** POWER ON ***");
          z80->power_on(0xf000);
          break;
        }
    }
};

__force_link(CMD_reset);
