/*
 *  KCemu -- the KC 85/3 and KC 85/4 Emulator
 *  Copyright (C) 1997-2003 Torsten Paul
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

#include <string.h>
#include <fstream>
#include <iostream>

#include "kc/system.h"

#include "kc/kc.h"
#include "kc/fdc.h"
#include "kc/disk.h"
#include "kc/mod_cpm.h"

using namespace std;

ModuleCPMZ9::ModuleCPMZ9(ModuleCPMZ9 &tmpl) :
  ModuleInterface(tmpl.get_name(), tmpl.get_id(), tmpl.get_type())
{
  fdc_fdc = new FDC(); // FIXME: global variable in kc.cc !!!
  disk = new Disk(); // FIXME: global variable in kc.cc !!!

  _portg1 = ports->register_ports(get_name(), 0x98, 2, fdc_fdc, 0);
  _portg2 = ports->register_ports(get_name(), 0xa0, 2, fdc_fdc, 0);
}

ModuleCPMZ9::ModuleCPMZ9(const char *name) :
  ModuleInterface(name, 0, KC_MODULE_KC_85_1)
{
  _portg1 = _portg2 = NULL;
}

ModuleCPMZ9::~ModuleCPMZ9(void)
{
  if (_portg1)
    ports->unregister_ports(_portg1);
  if (_portg2)
    ports->unregister_ports(_portg2);

  if (disk)
    delete disk;
  if (fdc_fdc)
    delete fdc_fdc;

  disk = NULL;
  fdc_fdc = NULL;
}

void
ModuleCPMZ9::m_out(word_t addr, byte_t val)
{
}

ModuleInterface *
ModuleCPMZ9::clone(void)
{
  return new ModuleCPMZ9(*this);
}

byte_t
ModuleCPMZ9::in(word_t addr)
{
  cout << "ModuleCPMZ9::in(): addr = 0x" << hex << (int)addr << endl;
  return 0xff;
}

void
ModuleCPMZ9::out(word_t addr, byte_t val)
{
  cout << "ModuleCPMZ9::out(): addr = 0x" << hex << (int)addr << " / " << (int)val << endl;
}
