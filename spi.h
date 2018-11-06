/* $Id: spi.h 14 2006-09-17 09:43:34Z wojtekka $ */

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

#ifndef __SPI_H
#define __SPI_H

#include <avr/io.h>
#include <inttypes.h>
#include "compat.h"

#define spi_sck_set() sbi(PORTD, 1)
#define spi_sck_clear() cbi(PORTD, 1)
#define spi_mosi_set() sbi(PORTD, 3)
#define spi_mosi_clear() cbi(PORTD, 3)
#define spi_miso_get() bit_is_set(PIND, 4)

extern void spi_write(uint8_t);
extern uint8_t spi_read(void);

#endif /* __SPI_H */
