// Commander X16 Emulator
// Copyright (c) 2019 Michael Steil
// All rights reserved. License: 2-clause BSD

#include <inttypes.h>

#define UARTCARD_RX 0x009FC0
#define UARTCARD_TX 0x009FC1

extern FILE *uart_in_file;
extern FILE *uart_out_file;

void vera_uart_init();
void vera_uart_step();
uint8_t vera_uart_read(uint8_t address);
void vera_uart_write(uint8_t address, uint8_t value);
