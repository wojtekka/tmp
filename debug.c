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

#include <avr/io.h>
#include <avr/delay.h>
#include "debug.h"

#ifdef DEBUG

/*
 * 9600bps @ 6MHz
 */
#define debug_delay() _delay_loop_1(208)

/*
 * debug_send()
 *
 * sends single byte to the host via SPI interface.
 */
void debug_send(uint8_t data)
{
	uint8_t i;

	debug_txd_clear();
	debug_delay();

	for (i = 8; i; i--) {
		if ((data & 1))
			debug_txd_set();
		else
			debug_txd_clear();

		debug_delay();

		data >>= 1;
	}

	debug_txd_set();
	debug_delay();
	debug_delay();
}

/*
 * debug_send_nibble()
 *
 * sends one hex nibble (0..F) using debug_send()
 */
static void debug_send_nibble(uint8_t x)
{
	if (x < 10)
		debug_send(x + '0');
	else
		debug_send(x + 'A' - 10);
}

/*
 * debug_send_hex()
 *
 * sends hex value using debug_send()
 */
void debug_send_hex(uint8_t data)
{
	debug_send_nibble((data >> 4) & 15);
	debug_send_nibble(data & 15);
}

#endif /* DEBUG */

