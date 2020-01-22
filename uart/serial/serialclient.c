//
//  serialclient.c
//  CommanderX16
//
//  Created by Pearce, Matthew (Senior Developer) on 21/01/2020.
//

#include "serialclient.h"

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <pthread.h>
#include "uartqueue.h"
#include <sys/ioctl.h>

int fd; /* File descriptor for the port */
pthread_t tid;
speed_t baud = B2400; /* baud rate */
char *serial_device = "/dev/null";
void *process_serial(void *vargp) ;
void serial_read();
/*
 * 'open_port()' - Open serial port 1.
 *
 * Returns the file descriptor on success or -1 on error.
 */

int
serial_connect(void)
{
	fd = open(serial_device, O_RDWR | O_NOCTTY | O_NDELAY);

	/* set the other settings (in this case, 9600 8N1) */
	struct termios settings;
	tcgetattr(fd, &settings);

	cfsetospeed(&settings, baud); /* baud rate */
	settings.c_cflag &= ~PARENB; /* no parity */
	settings.c_cflag &= ~CSTOPB; /* 1 stop bit */
	settings.c_cflag &= ~CSIZE;
	settings.c_cflag |= CS8 | CLOCAL; /* 8 bits */
	settings.c_lflag = ICANON; /* canonical mode */
	settings.c_oflag &= ~OPOST; /* raw output */

	tcsetattr(fd, TCSANOW, &settings); /* apply the settings */
	tcflush(fd, TCOFLUSH);

	if (fd >= 0) {
		fcntl(fd, F_SETFL, 1);
		pthread_create(&tid, NULL, process_serial, (void *)&tid);
	}

	return fd;
}

void *process_serial(void *vargp)  {

	for(;;) {

		if (get_outgoing_queue_length() > 0) {
			uint8_t value = get_outgoing_value();
			serial_write(value);
		} else {
			int bytes;

			ioctl(fd, FIONREAD, &bytes);
			if (get_incoming_queue_length() < MAX_ITEMS && bytes > 0) {
				serial_read();
			}
		}
	}
}

size_t
serial_write(uint8_t in_value) {
	size_t n = write(fd, &in_value, sizeof(in_value));
	return n;
}


void serial_read(char *serial_byte) {

	char message[255];
	serial_byte = message;
	size_t bytes_recv;

	while ((bytes_recv = read(fd, serial_byte, sizeof(message))) > 0)
	  {
		  for (int i = 0; i < bytes_recv; i++) {
			  insert_incoming_value(serial_byte[i]);
		  }
		  serial_byte += bytes_recv;
		  if (serial_byte[-1] == '\n' || serial_byte[-1] == '\r')
			break;
	  }
}

void serial_close(void) {
	pthread_cancel(tid);
	close(fd);
}

