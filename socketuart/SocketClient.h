//
//  SocketClient.h
//  CommanderX16
//
//	; (C)2020 Matthew Pearce, License: 2-clause BSD//

#ifndef SocketClient_h
#define SocketClient_h

#include <stdio.h>
#include <stdint.h>
void socket_connect(const char *hostAddress, int portno);
size_t socket_write(uint8_t in_value);
uint8_t socket_read(void);

#endif /* SocketClient_h */
