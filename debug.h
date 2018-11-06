/* $Id$ */

/*
 *  (C) Copyright 2003-2005 Wojtek Kaniewski <wojtekka@irc.pl>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License Version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __DEBUG_H
#define __DEBUG_H

#ifndef DEBUG

#define debug_send(x)
#define debug_send_hex(x)

#else /* DEBUG */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <inttypes.h>

#define debug_txd_set() sbi(PORTB, 6)
#define debug_txd_clear() cbi(PORTB, 6)

extern void debug_send(uint8_t data);
extern void debug_send_hex(uint8_t data);
#define debug_send_str(x) _debug_send_str(PSTR(x))
extern void _debug_send_str(const char *str);

#endif /* DEBUG */

#endif /* __DEBUG_H */
