/*
 *  KCemu -- the KC 85/3 and KC 85/4 Emulator
 *  Copyright (C) 1997-2001 Torsten Paul
 *
 *  $Id: ctc_fdc.cc,v 1.5 2002/01/02 00:35:05 torsten_paul Exp $
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

#include <iostream.h>
#include <iomanip.h>

#include "kc/config.h"
#include "kc/system.h"

#include "kc/kc.h"
#include "kc/ctc_fdc.h"
#include "kc/z80_fdc.h"

#include "libdbg/dbg.h"

/*
 *  quoting the D004 programming manual:
 * 
 *  Alle vier Kan�le des CTC sind hardwarem��ig kaskadiert. Die
 *  Z�hlereingangsfrequenz des Kanals 0 betr�gt 500kHz. Der Kanal 3 ist
 *  vom Betriebssystem interruptm��ig zur Laufwerksteuerung belegt. Die
 *  Kan�le 0 und 1 werden von der Systemuhr verwendet. Kanal 1 arbeitet
 *  dabei interruptgesteuert.
 *
 *  Die Adressen der Interruptserviceroutinen sind folgender Tabelle zu
 *  entnehmen:
 *
 *  CTC-Kanal  Adresse  Wert
 *
 *   0          FBE0h   0000h
 *   1          FBE2h   F0e4h
 *   2          FBE4h   0000h
 *   3          FBE6h   F4C8h
 */

CTC_FDC::CTC_FDC(void)
{
}

CTC_FDC::~CTC_FDC(void)
{
}

long long
CTC_FDC::get_counter()
{
  return fdc_z80->get_counter();
}

byte_t
CTC_FDC::trigger_irq(byte_t irq_vector)
{
  fdc_z80->trigger_irq(irq_vector);
  return 1;
}

void
CTC_FDC::add_callback(unsigned long long offset, Callback *cb, void *data)
{
  fdc_z80->add_callback(offset, cb, data);
}

byte_t
CTC_FDC::in(word_t addr)
{
  DBG(2, form("KCemu/CTC/FDC/in",
              "CTC_FDC::in(): addr = %04x\n",
              addr));

  switch (addr & 3)
    {
    case 0:
      return c_in(0);
    case 1:
      return c_in(1);
    case 2:
      return c_in(2);
    case 3:
      return c_in(3);
    }
}

void
CTC_FDC::out(word_t addr, byte_t val)
{
  DBG(2, form("KCemu/CTC/FDC/out",
              "CTC_FDC::out(): addr = %04x, val = %02x\n",
              addr, val));
  switch (addr & 3)
    {
    case 0:
      c_out(0, val);
      break;
    case 1:
      c_out(1, val);
      break;
    case 2:
      c_out(2, val);
      break;
    case 3:
      c_out(3, val);
      break;
    }
}

bool
CTC_FDC::irq_0(void)
{
  DBG(2, form("KCemu/CTC_FDC/irq/0",
	      "CTC::irq_0()\n"));
  trigger(1);
  return true;
}

bool
CTC_FDC::irq_1(void)
{
  DBG(2, form("KCemu/CTC_FDC/irq/1",
	      "CTC::irq_1()\n"));
  return true;
}

bool
CTC_FDC::irq_2(void)
{
  DBG(2, form("KCemu/CTC_FDC/irq/2",
	      "CTC::irq_2()\n"));
  return true;
}

bool
CTC_FDC::irq_3(void)
{
  DBG(2, form("KCemu/CTC_FDC/irq/3",
	      "CTC::irq_3()\n"));
  return true;
}

long
CTC_FDC::counter_value_0(void)
{
  return 0;
}

long
CTC_FDC::counter_value_1(void)
{
  return 0;
}

long
CTC_FDC::counter_value_2(void)
{
  return 0;
}

long
CTC_FDC::counter_value_3(void)
{
  return 0;
}
