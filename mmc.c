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
#include "debug.h"
#include "mmc.h"
#include "player.h"
#include "spi.h"
#include "types.h"

#define mmc_write spi_write
#define mmc_write_0xff() spi_write(0xff)
#define mmc_read spi_read

uint8_t mmc_buf[32];
uint32_t mmc_addr;

/*
 * mmc_response()
 *
 * waits for MMC response and returns its value.
 */
static uint8_t mmc_response(void)
{
	uint8_t i, j;

	for (i = 255; i; i--) {
		j = mmc_read();

		if (j != 0xff)
			break;
	}

	return j;
}

/*
 * mmc_write_command()
 *
 * sends MMC command with no parameters.
 */
static void mmc_write_command(uint8_t command)
{
	uint8_t i;
	
	mmc_write(command);

	for (i = 4; i; i--)
		mmc_write(0x00);

	mmc_write(0x95);
}

/*
 * mmc_init()
 *
 * initializes MMC card.
 */
void mmc_init(void)
{
	uint8_t i;

	mmc_cs_set();

	for (i = 10; i; i--)
		mmc_write_0xff();

	mmc_cs_clear();

	mmc_write_command(MMC_GO_IDLE_STATE);

	if (mmc_response() != 0x01)
		player_error(0);

	for (i = 255; i; i--) {
		mmc_cs_set();
		mmc_write_0xff();
		mmc_cs_clear();

		mmc_write_command(MMC_SEND_OP_COND);

		if (!mmc_response())
			break;
	}

	if (!i)
		player_error(1);

	mmc_cs_set();
}

/*
 * mmc_read_block_start()
 *
 * seeks to `mmc_addr' waiting for MMC to be ready.
 */
void mmc_read_block_start(void)
{
	uint8_t i;
	
	mmc_write_0xff();
	mmc_write_0xff();

	mmc_cs_clear();

	mmc_write(MMC_READ_SINGLE_BLOCK);
	mmc_write(byte(mmc_addr, 3));
	mmc_write(byte(mmc_addr, 2));
	mmc_write(byte(mmc_addr, 1) & 0xfe);
	mmc_write(0);

	if (mmc_response() != 0x00)
		player_error(2);

	for (;;) {
		i = mmc_read();

		if (i == 0xfe)
			return;

		if (i != 0xff)
			break;
	}

	debug_send('r');
	debug_send_hex(i);

	player_error(3);
}

void mmc_read_block_end(void)
{
	mmc_read();	/* CRC */
	mmc_read();

	mmc_cs_set();
}

/*
 * mmc_read_block_raw()
 *
 * reads 32 bytes from MMC card and saves them to `mmc_buf'.
 */
void mmc_read_block_raw(void)
{
	uint8_t i, j;

	for (i = 32, j = 0; i; i--, j++)
		mmc_buf[j] = mmc_read();
}

/*
 * mmc_read_block()
 *
 * reads MMC block by seeking to 512 byte boundary, skipping some bytes,
 * saving 32 bytes to `mmc_buf' and reading the rest to keep MMC protocol
 * in sync.
 */
void mmc_read_block(uint32_t addr)
{
	uint16_t skip, all = 512;

	mmc_addr = addr;

	skip = addr & 511;

	mmc_read_block_start();

	while (skip--) {
		mmc_read();
		all--;
	}

	mmc_read_block_raw();
	all -= 32;

	while (all--)
		mmc_read();

	mmc_read_block_end();
}

