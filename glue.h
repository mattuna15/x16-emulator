// Commander X16 Emulator
// Copyright (c) 2019 Michael Steil
// All rights reserved. License: 2-clause BSD

#ifndef _GLUE_H_
#define _GLUE_H_

#include <stdint.h>
#include <stdbool.h>

//#define TRACE
//#define PERFSTAT
#define LOAD_HYPERCALLS

#define MHZ 8

#define NUM_MAX_RAM_BANKS 256
#define NUM_ROM_BANKS 8

#define RAM_SIZE (0xa000 + num_ram_banks * 8192) /* $0000-$9FFF + banks at $A000-$BFFF */
#define ROM_SIZE (NUM_ROM_BANKS * 16384)   /* banks at $C000-$FFFF */

typedef enum {
	ECHO_MODE_NONE,
	ECHO_MODE_RAW,
	ECHO_MODE_COOKED,
	ECHO_MODE_ISO,
} echo_mode_t;

// GIF recorder commands
typedef enum {
	RECORD_GIF_PAUSE,
	RECORD_GIF_SNAP,
	RECORD_GIF_RESUME
} gif_recorder_command_t;

// GIF recorder states
typedef enum {
	RECORD_GIF_DISABLED,
	RECORD_GIF_PAUSED,
	RECORD_GIF_SINGLE,
	RECORD_GIF_ACTIVE
} gif_recorder_state_t;

extern uint8_t a, x, y, sp, status;
extern uint16_t pc;
extern uint8_t *RAM;
extern uint8_t ROM[];

extern uint16_t num_ram_banks;

extern bool debugger_enabled;
extern bool log_video;
extern bool log_keyboard;
extern echo_mode_t echo_mode;
extern bool save_on_exit;
extern gif_recorder_state_t record_gif;
extern char *gif_path;
extern uint8_t keymap;

#ifdef WITH_SOCKETS
extern char *ip_address;
extern int port;
#endif

#ifdef WITH_SERIAL
extern char *serial_device;
#endif

extern void machine_dump(void);
extern void machine_reset(void);
extern void machine_paste(char *s);
extern void init_audio(void);
extern void vera_uart_step(void);

#endif
