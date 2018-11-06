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

#include <avr/delay.h>
#include <avr/io.h>
#include "mmc.h"
#include "spi.h"
#include "vs1001k.h"
#include "player.h"

uint8_t vs_volume = 64;

/*
 * vs_dreq_wait()
 *
 * waits until the VS1001K will be ready for another 32 bytes of data.
 * when the loop times out, it reports an error.
 */
void vs_dreq_wait(void)
{
	uint16_t timeout = 65535;

	while (timeout && bit_is_clear(PINB, 2))
		timeout--;

	if (!timeout)
		player_error(4);
}

/*
 * vs_data_write()
 *
 * writes single byte to VS1001K, handling BSYNC properly, but ignoring DREQ.
 */
void vs_data_write(uint8_t data)
{
	uint8_t i;

	vs_bsync_set();

	for (i = 8; i; i--) {
		if ((data & 128))
			spi_mosi_set();
		else
			spi_mosi_clear();

		data <<= 1;
		
		vs_dclk_set();
		vs_dclk_clear();

		vs_bsync_clear();
	}
}

/*
 * vs_data_write_buf()
 *
 * writes `size' bytes from `mmc_buf' to VS1001K waiting for DREQ.
 */
void vs_data_write_buf(uint8_t size)
{
	uint8_t i;
	
	vs_dreq_wait();

	for (i = 0; size; i++, size--)
		vs_data_write(mmc_buf[i]);
}

/*
 * _vs_command_write()
 *
 * internal command to write VS1001K command. the correct function is
 * vs_command_write(uint8_t command, uint16_t value); but `value' has to
 * be a constant. it's an optimization hack.
 */
void _vs_command_write(uint8_t command, uint8_t hi, uint8_t lo)
{
	vs_cs_clear();
	spi_write(0x02);
	spi_write(command);
	spi_write(hi);
	spi_write(lo);
	vs_cs_set();
}

#if 0
/*
 * vs_beep()
 *
 * makes VS1001K beep, for debugging purposes.
 */
void vs_beep(void)
{
	uint8_t i;
	
	vs_data_write(0x53);
	vs_data_write(0xef);
	vs_data_write(0x6e);
	vs_data_write(0x30);

	for (i = 16; i; i--)
		vs_data_write(0x00);

	for (i = 10; i; i--)
		player_delay_10ms();

	vs_data_write(0x45);
	vs_data_write(0x78);
	vs_data_write(0x69);
	vs_data_write(0x74);

	for (i = 16; i; i--)
		vs_data_write(0x00);

	for (i = 10; i; i--)
		player_delay_10ms();
}
#endif

/*
 * vs_volume_set()
 *
 * sets VS1001K volume. note that it's in logarithmic scale.
 */
void vs_volume_set(uint8_t volume)
{
	_vs_command_write(VS_VOL, volume, volume);
}

/*
 * vs_reset()
 *
 * resets VS1001K's decoder. required between separate mp3 streams.
 */
void vs_reset(void)
{
	uint8_t i, j;

	vs_volume_mute();

	for (i = 0, j = 32; j; i++, j--)
		mmc_buf[i] = 0;

	for (i = 64; i; i--)
		vs_data_write_buf(32);

	for (i = 10; i; i--)
		player_delay_10ms();

	vs_command_write(VS_MODE, VS_SM_RESET);

	_delay_loop_1(4);		/* 7.4. Wait at least 2us */

	vs_dreq_wait();

	vs_data_write(0);

	vs_volume_set(vs_volume);
}

/*
 * vs_init()
 *
 * initializes VS1001K, sets clock frequency.
 */
void vs_init(void)
{
	/* RESET has been cleared earlier */

	vs_reset_set();
	player_delay_10ms();

	vs_dreq_wait();

	vs_command_write(VS_CLOCKF, VS_CLOCKF_VALUE);
}

