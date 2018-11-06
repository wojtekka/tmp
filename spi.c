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

#include "debug.h"
#include "player.h"
#include "spi.h"
#include "vs1001k.h"

/*
 * spi_read()
 *
 * reads single byte from software SPI interface.
 */
uint8_t spi_read()
{
	uint8_t i, j;

	spi_mosi_set();

	for (j = 0, i = 8; i; i--) {
		j <<= 1;

		spi_sck_set();
		if (spi_miso_get())
			j |= 1;
		spi_sck_clear();
	}

	return j;
}

/*
 * spi_send()
 *
 * writes single byte to software SPI interface.
 */
void spi_write(uint8_t data)
{
	uint8_t i;

	for (i = 8; i; i--) {
		if ((data & 128))
			spi_mosi_set();
		else
			spi_mosi_clear();

		spi_sck_set();
		spi_sck_clear();

		data <<= 1;
	}
}

