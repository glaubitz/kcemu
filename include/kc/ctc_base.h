/*
 *  KCemu -- the KC 85/3 and KC 85/4 Emulator
 *  Copyright (C) 1997-2001 Torsten Paul
 *
 *  $Id: ctc_base.h,v 1.2 2001/12/31 14:11:53 torsten_paul Exp $
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

#ifndef __kc_ctc_base_h
#define __kc_ctc_base_h

#include <list.h>

#include "kc/ctc.h"

class CTC_Base : public CTC
{
 protected:
  virtual long long get_counter();
  virtual byte_t trigger_irq(byte_t irq_vector);
  virtual void add_callback(unsigned long long offset, Callback *cb, void *data);

 public:
  CTC_Base(void);
  virtual ~CTC_Base(void);
};

#endif /* __kc_ctc_base_h */
