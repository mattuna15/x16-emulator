//
//  serialclient.h
//  CommanderX16
//
//  Created by Pearce, Matthew (Senior Developer) on 21/01/2020.
//

#ifndef serialclient_h
#define serialclient_h

#include <stdio.h>
#include <stdint.h>

int serial_connect(void);
size_t serial_write(uint8_t in_value);

void serial_close(void);

#endif /* serialclient_h */
