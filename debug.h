/* $Id$ */

/*
 *  (C) Copyright 2003 Wojtek Kaniewski <wojtekka@irc.pl>
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
#include <inttypes.h>

#define debug_miso_set() sbi(PORTB, 6)
#define debug_miso_clear() cbi(PORTB, 6)
#define debug_loop_until_sck_is_set() loop_until_bit_is_set(PINB, 7)
#define debug_loop_until_sck_is_clear() loop_until_bit_is_clear(PINB, 7)

extern void debug_send(uint8_t data);
extern void debug_send_hex(uint8_t data);

#endif /* DEBUG */

#endif /* __DEBUG_H */
