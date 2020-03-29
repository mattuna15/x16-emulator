/*
 * midi.h - MIDI (6850 UART) emulation.
 *
 * Written by
 *  Hannu Nuotio <hannu.nuotio@tut.fi>
 *
 * Based on code by
 *  Andre Fachat <a.fachat@physik.tu-chemnitz.de>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#ifndef VICE_MIDI_H
#define VICE_MIDI_H

#include "types.h"

/* Emulated interfaces */
enum {
    MIDI_MODE_SEQUENTIAL = 0,   /* Sequential Circuits Inc. */
    MIDI_MODE_PASSPORT,         /* Passport & Syntech */
    MIDI_MODE_DATEL,            /* DATEL/Siel/JMS */
    MIDI_MODE_NAMESOFT,         /* Namesoft */
    MIDI_MODE_MAPLIN            /* Electronics - Maplin magazine */
};

struct midi_interface_s {
    /* Name of the interface */
    char *name;
    /* Base address (C64 specific) */
    uint16_t base_addr;
    /* Control register address */
    uint16_t ctrl_addr;
    /* Status register address */
    uint16_t status_addr;
    /* Transmit register address */
    uint16_t tx_addr;
    /* Receive register address */
    uint16_t rx_addr;
    /* Address mask (for mirroring) */
    uint16_t mask;
    /* Correct counter divide for 31250 bps */
    uint8_t midi_cd;
    /* Interrupt type: none (0), IRQ (1) or NMI (2) */
    int irq_type;
    /* cart-ID of associated cartridge/type of expansion, 0 means internal */
    int cartid;
};
typedef struct midi_interface_s midi_interface_t;

#define MIDI_INFERFACE_LIST_END { NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0 }

#define MIDI_DATA_STATUS_EMPTY 0
#define MIDI_DATA_STATUS_DATA_AVAIL 1
#define MIDI_DATA_BUSY 2

extern midi_interface_t midi_interface[];

extern int midi_enabled;
extern int midi_mode;

extern void midi_init(void);
extern void midi_reset(void);
extern int midi_set_mode(int new_mode, void *param);

extern uint8_t midi_read(uint16_t a);
extern uint8_t midi_peek(uint16_t a);
extern void midi_store(uint16_t a, uint8_t b);
/* returns 1 if address is a readable MIDI register */
extern int midi_test_read(uint16_t a);
/* returns 1 if address is any MIDI register */
extern int midi_test_peek(uint16_t a);

extern int midi_resources_init(void);
extern void midi_resources_shutdown(void);

/* MIDI device I/O */
/* return: -1 if error, 1 if a byte was read to *b, 0 if no new bytes */
extern int mididrv_in(uint8_t *b);
extern void mididrv_out(uint8_t b);

void midi_put_byte(uint8_t b);
void midi_get_byte(uint8_t *b);
extern int midi_data_status;

#endif
