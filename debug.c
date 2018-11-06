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

#include <avr/io.h>
#include "debug.h"

#ifdef DEBUG

/*
 * debugging protocol for the host:
 *
 * 1) wait for rising edge on MISO,
 * 2) cycle SCK,
 * 3) read bit (from MSB to LSB),
 * 4) go to 2, until whole byte is read.
 */

/*
 * debug_send()
 *
 * sends single byte to the host via SPI interface.
 */
void debug_send(uint8_t data)
{
	uint8_t i;

	debug_miso_set();

	for (i = 8; i; i--, data <<= 1) {
		debug_loop_until_sck_is_set();
		
		if ((data & 128))
			debug_miso_set();
		else
			debug_miso_clear();
		
		debug_loop_until_sck_is_clear();
	}

	debug_miso_clear();
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

