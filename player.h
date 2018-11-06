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

#ifndef __PLAYER_H
#define __PLAYER_H

#include <avr/io.h>
#include <inttypes.h>

#define player_play_pressed() bit_is_clear(PIND, 2)
#define player_play_pressed_wait() loop_until_bit_is_set(PIND, 2)

#define player_volp_pressed() bit_is_clear(PIND, 0)

#define player_led_set() sbi(PORTD, 6)
#define player_led_clear() cbi(PORTD, 6)

#define nop() __asm__ __volatile__ ("nop\n\t")

extern void player_error(uint8_t val);
extern void player_delay_10ms(void);

#endif /* __PLAYER_H */
