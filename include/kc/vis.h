/*
 *  KCemu -- the KC 85/3 and KC 85/4 Emulator
 *  Copyright (C) 1997-2001 Torsten Paul
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

#ifndef __kc_vis_h
#define __kc_vis_h

#include "kc/system.h"

#include "kc/ic.h"
#include "kc/ports.h"

class VIS : public PortInterface, public InterfaceCircuit
{
 private:
  int _idx;
  int _mode;
  int _border;
  byte_t _char[0x0800];

 public:
  VIS(void);
  virtual ~VIS(void);

  virtual byte_t in(word_t addr);
  virtual void out(word_t addr, byte_t val);

  virtual int get_mode();
  virtual void set_mode(int mode);
  virtual int get_border();
  virtual void set_border(int border);
  virtual byte_t * get_memory();

  /*
   *  InterfaceCircuit
   */
  virtual void reti(void);
  virtual void irqreq(void);
  virtual word_t irqack(void);
  virtual void reset(bool power_on = false);
};

#endif /* __kc_vis_h */
