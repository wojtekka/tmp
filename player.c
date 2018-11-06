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

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/signal.h>
#include <avr/sleep.h>
#include "debug.h"
#include "mmc.h"
#include "player.h"
#include "spi.h"
#include "types.h"
#include "vs1001k.h"

uint8_t spc;			/* sectors per cluster */
uint32_t fat;			/* first fat offset */
uint32_t start;			/* first cluster offset */
uint8_t fat12;			/* is card formatted as fat12? */

/* the interrupt is used only to get AVR out of sleep mode */
EMPTY_INTERRUPT(SIG_INTERRUPT0);

/*
 * player_sleep()
 *
 * puts AVR in sleep mode between mp3 tracks.
 */
void player_sleep(void)
{
	/* turn off the led */
	player_led_clear();

	/* power down vs1001k */
	vs_reset_clear();

	/* enable INT0 to wake up */
	sei();
	
	/* power down the avr */
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_mode();

	/* disable INT0 while playing */
	cli();
}

/*
 * player_error()
 *
 * indicates that something's wrong. at the moment it sends debug messages
 * using SPI interface to the host.
 */
void player_error(uint8_t val)
{
	mmc_cs_set();

	debug_send('!');
	debug_send(val + 48);

	/* software reset */
	__asm__ __volatile__ ("rjmp __vectors" ::);
}

/*
 * player_delay_10ms()
 *
 * simple delay used for debouncing. doesn't have to be accurate.
 */
void player_delay_10ms(void)
{
	uint8_t i, j;

	for (i = 255; i; i--)
		for (j = 255; j; j--)
			nop();
}

/*
 * fat_next()
 *
 * finds next cluster.
 */
uint16_t fat_next(uint16_t cluster)
{
	uint32_t block;
	uint8_t index;
		
	if (fat12) {
		uint8_t b1, b2;

		block = cluster;		/* block = cluster * 3 / 2; */
		block <<= 1;
		block += cluster;
		block >>= 1;
		
		index = block & 31;
		block += fat;
		byte(block, 0) &= 0xe0;		/* block &= 0xffffffe0; */
		
		mmc_read_block(block);

		b1 = mmc_buf[index];

		if (index == 31) {
			block += 32;
			mmc_read_block(block);
			b2 = mmc_buf[0];
		} else
			b2 = mmc_buf[++index];

		if (cluster & 1) {
			return ((b1 & 0xf0) >> 4) | (b2 << 4);
		} else {
			return b1 | ((b2 & 0x0f) << 8);
		}
	} else {
		block = cluster << 1;
		index = block & 31;
		block += fat;
		byte(block, 0) &= 0xe0;		/* block &= 0xffffffe0; */

		mmc_read_block(block);
		
		return (mmc_buf[index + 1] << 8) | mmc_buf[index];
	}
}

/*
 * play()
 *
 * starts playing file from specified cluster.
 */
void play(uint16_t cluster, int32_t size)
{
	uint8_t i, j, finished = 0, vol_pressed = 0;

	vs_reset();

#if 0
	debug_send_hex((cluster >> 8) & 255);
	debug_send_hex(cluster & 255);
	debug_send(' ');
#endif
	
	finished = 0;

	for (;;) {
		for (mmc_addr = 0, i = spc; i; i--) {	/* mmc_addr =       */
			mmc_addr += cluster;		/*   (cluster - 2)  */
			mmc_addr -= 2;			/*   * spc;         */
		}

		mmc_addr <<= 9;
		mmc_addr += start;

#if 0
		debug_send('o');
		debug_send_hex(mmc_addr >> 24 & 255);
		debug_send_hex(mmc_addr >> 16 & 255);
		debug_send_hex(mmc_addr >> 8 & 255);
		debug_send_hex(mmc_addr & 255);
#endif
		
		for (i = spc; i; i--) {
			if (player_volp_pressed()) {
				if (!vol_pressed) {
					vol_pressed = 1;
					if (vs_volume > 4)
						vs_volume -= 4;
#ifndef VOLUME_UP_DOWN
					else
						vs_volume = 108;
#endif
					vs_volume_set(vs_volume);
				}
			} else {
				vol_pressed = 0;
			}

#ifdef VOLUME_UP_DOWN
			if (player_volm_pressed()) {
				if (!vol_pressed) {
					vol_pressed = 1;
					if (vs_volume < 108)
						vs_volume += 4;
					vs_volume_set(vs_volume);
				}
			} else {
				vol_pressed = 0;
			}
#endif

			mmc_read_block_start();

			for (j = 16; j; j--) {
				uint8_t bsize = 32;

				mmc_read_block_raw();

				if (size > 32)
					bsize = 32;
				else {
					bsize = byte(size,0);
					finished = 1;
				}

				if (player_play_pressed())
					finished = 1;

				vs_data_write_buf(bsize);

				size -= bsize;
			}
			
			mmc_read_block_end();

			mmc_addr += 512;

			if (finished)
				return;
		}

		/* read next cluster offset */
		cluster = fat_next(cluster);

#if 0
		debug_send_hex(byte(cluster, 1));
		debug_send_hex(byte(cluster, 0));
		debug_send(' ');
#endif

#if 0
		if (fat12) {
			if ((cluster & 0x0ff0) == 0x0ff0)
				break;
		} else {
			if ((cluster & 0xfff0) == 0xfff0)
				break;
		}
#endif
	}
}

int main(void)
{
	uint16_t dirsize;	/* root directory entries count */
	uint32_t directory;	/* root directory offset */

//	      7       0
//	PORTB x010 1010	0x2a
//	DDRB  0101 1011	0x5b
//	      smvb cdrd
//            coos srsc
//            lsly ~etl
//	      ki-n  q~k
//	         
//	PORTD  011 0101	0x35
//	DDRD   110 1010	0x6a
//             lms spsv
//             emo ilco
//             dc   all
//	        s   yk+

	/* setup ports */
	PORTB = 0x2a;
	DDRB = 0x5b;
	PORTD = 0x35;
	DDRD = 0x6a;
	
	/* setup Play/Next button interrupt */
	enable_external_int(_BV(INT0));

sleep:
	/* power down until Play/Next pressed */
	player_sleep();

	/* initialize peripherials */
	mmc_init();
	vs_init();

	/* turn on status LED */
	player_led_set();

	/* read the partition table */
	mmc_read_block(0x1c0);

	/* find the beginning of the 1st partition */
	start = ((struct partition*) mmc_buf)->start;
	start <<= 9;

	/* read bootsector */
	mmc_read_block(start);

	spc = mmc_buf[0x0d];			/* sectors per cluster */

	/* find fat offset */
	fat = *((uint16_t*)&mmc_buf[0x0e]);	/* reserved sectors */
	fat <<= 9;
	fat += start;

	/* find root directory offset */
	directory = *((uint16_t*)&mmc_buf[0x16]);	/* sectors per fat */

	/* if fat isn't longer than 4096 entries, we assume it's fat12 */
	if (directory <= 0x0c)
		fat12 = 1;
	
	directory <<= 10;			/* 512B/sector, 2 fats */
	
	directory += fat;

	dirsize = *((uint16_t*)&mmc_buf[0x11]);	/* max root dir entries */
	
	/* offset of the first cluster */
	start = dirsize << 5;			/* dirsize * 32 */
	start += directory;

	/* play every file from the root directory */
	for (; dirsize; dirsize--, directory += 32) {
		mmc_read_block(directory);

		/* empty entry ends directory */
		if (!mmc_buf[0])
			break;
		
		/* skip volume id, directories, system and hidden files */
		if ((mmc_buf[11] & 0x1e))
			continue;

		/* skip deleted files and those without .mp3 extension */
		if (mmc_buf[0] == 229 || mmc_buf[8] != 'M' || mmc_buf[9] != 'P' || mmc_buf[10] != '3')
			continue;
		
		/* wait until play/next button is depressed */
pressed:
		if (player_play_pressed()) {
			while (player_play_pressed())
				;
			player_delay_10ms();
			if (player_play_pressed())
				goto pressed;
		}
		
		play(*((uint16_t*)&mmc_buf[0x1a]), *((uint32_t*)&mmc_buf[0x1c]));
	}

	/* flush the buffer, reset etc. */
	vs_reset();

	goto sleep;
}
