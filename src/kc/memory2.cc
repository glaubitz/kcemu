/*
 *  KCemu -- the KC 85/3 and KC 85/4 Emulator
 *  Copyright (C) 1997-2001 Torsten Paul
 *
 *  $Id: memory2.cc,v 1.3 2001/12/31 14:11:53 torsten_paul Exp $
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
#include <stdlib.h>
#include <fstream.h>

#include "kc/config.h"
#include "kc/system.h"

#include "kc/kc.h"
#include "kc/z80.h"
#include "kc/memory2.h"

#include "ui/ui.h"

Memory2::Memory2(void) : Memory()
{
  int l;
  char *ptr;
  struct {
    MemAreaGroup **group;
    const char    *name;
    word_t         addr;
    dword_t        size;
    byte_t        *mem;
    int            prio;
    bool           ro;
    bool           active;
  } *mptr, m[] = {
    { &_m_scr,   "-",     0x0000, 0x10000, 0,            256, 0, 1 },
    { &_m_ram,   "RAM 0", 0x0000,  0x4000, &_ram[0],       0, 0, 1 },
    { &_m_irm,   "IRM",   0x8000,  0x4000, &_irm[0],       1, 0, 1 },
    { &_m_caos,  "CAOS",  0xe000,  0x2000, &_rom_caos[0],  2, 1, 1 },
    { 0, },
  };
  
  l = strlen(kcemu_datadir);
  ptr = new char[l + 14];
  strcpy(ptr, kcemu_datadir);
  
  strcpy(ptr + l, "/caos__e0.852");
  loadROM(ptr, &_rom_caos, 0x2000, 1);
  
  _access_color = false;
  
  for (mptr = &m[0];mptr->name;mptr++)
    {
      *(mptr->group) = new MemAreaGroup(mptr->name,
                                        mptr->addr,
                                        mptr->size,
                                        mptr->mem,
                                        mptr->prio,
                                        mptr->ro);
      (*(mptr->group))->add(get_mem_ptr());
      if (mptr->active)
        (*(mptr->group))->set_active(true);
    }
  
  reload_mem_ptr();
  
  reset(true);
  z80->register_ic(this);
}

Memory2::~Memory2(void)
{
  z80->unregister_ic(this);
}

byte_t *
Memory2::getIRM(void)
{
  return (byte_t *)_irm;
}

void
Memory2::dumpCore(void)
{
#if 0
  ofstream os;
    
  os.open("core.z80");

  cout.form("Memory: dumping core...\n");
  if (!os)
    {
      cerr << "can't write 'core.z80'\n";
      exit(0);
    }
  
  os.write(&_ram[0], 0x4000);
  os.write(&_irm[0],   0x4000);

  os.close();
  cout.form("Memory: done.\n");
#endif
}

void
Memory2::reset(bool power_on)
{
  if (!power_on)
    return;

  scratch_mem(&_ram[0],  0x4000);
  scratch_mem(&_irm[0],  0x4000);
}
