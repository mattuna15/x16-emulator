//
//  SocketClient.c
//  CommanderX16
//
//	; (C)2020 Matthew Pearce, License: 2-clause BSD//

#include "SocketClient.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include "UartQueue.h"
#include <string.h>
#include<arpa/inet.h>	//inet_addr

char *convert_char(uint8_t *a);
int sockfd;
int connected;

void *processmessages(void *vargp) ;

void socket_connect(const char *hostAddress, int portno)
{
	//struct hostent *he;
	struct sockaddr_in their_addr; /* connector's address information */

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	their_addr.sin_family = AF_INET;      /* host byte order */
	their_addr.sin_port = htons(portno);    /* short, network byte order */
	their_addr.sin_addr.s_addr = inet_addr(hostAddress);

	int optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval));

	connected = connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr));

	if (connected >= 0) {
		pthread_t tid;
		pthread_create(&tid, NULL, processmessages, (void *)&tid);
	}

}

void *processmessages(void *vargp)  {

	for(;;) {

		if (get_outgoing_queue_length() > 0) {
			uint8_t value = get_outgoing_value();
			size_t bytes_written = socket_write(value);
			if (bytes_written <= 0) {
				insert_outgoing_value(value);
			}
		} else {
			insert_incoming_value(socket_read());
		}

	}

}


size_t socket_write(uint8_t in_value) {

	size_t bytes_sent =  send(sockfd, &in_value, sizeof(in_value), 0);
	return bytes_sent;
}

uint8_t socket_read(void) {

	char reply_message[1];
	size_t bytes_recv = recv(sockfd, reply_message, 1, 0);

	if (bytes_recv <= 0) {
		return bytes_recv;
	} else {
		uint8_t value = (uint8_t) reply_message[0];
		return value;
	}

}
