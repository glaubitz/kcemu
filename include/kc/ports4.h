/*
 *  KCemu -- the KC 85/3 and KC 85/4 Emulator
 *  Copyright (C) 1997-2001 Torsten Paul
 *
 *  $Id: ports4.h,v 1.7 2002/06/15 18:32:31 torsten_paul Exp $
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

#ifndef __kc_ports4_h
#define __kc_ports4_h

#include "kc/ports.h"

class Ports4 : public PortInterface
{
public:
  enum {
    NR_PORTS = 256,
  };
  
protected:
  byte_t inout[NR_PORTS];

protected:
  virtual void change_0x84(byte_t changed, byte_t val);
  virtual void change_0x86(byte_t changed, byte_t val);
  
public:
  Ports4(void);
  virtual ~Ports4(void);
  virtual byte_t in(word_t addr);
  virtual void out(word_t addr, byte_t val);
};

#endif /* __kc_ports4_h */
