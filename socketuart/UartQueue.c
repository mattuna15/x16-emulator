//
//  UartQueue.c
//  CommanderX16
//
//  Created by Pearce, Matthew (Senior Developer) on 16/01/2020.
//

#include "UartQueue.h"


# define MAX_ITEMS 1024
uint8_t c_incoming_queue_arr[MAX_ITEMS];
uint8_t c_outgoing_queue_arr[MAX_ITEMS];
int incoming_front = -1;
int incoming_rear = -1;
int outgoing_front = -1;
int outgoing_rear = -1;

int get_outgoing_queue_length(void) {

	if(outgoing_front == -1) {
		printf("Queue is empty");
		return 0;
	}

	return outgoing_rear + 1;

}

int get_incoming_queue_length() {

	if(incoming_front == -1) {
		printf("Queue is empty");
		return 0;
	}

	return incoming_rear + 1;

}

bool insert_outgoing_value(uint8_t item)
{
	if ((outgoing_front == 0 && outgoing_rear == MAX_ITEMS - 1) || (outgoing_front == outgoing_rear + 1)) {
		printf("Queue Overflow");
		return false;
	}

	if (outgoing_front == -1) {
		outgoing_front = 0;
		outgoing_rear = 0;
	} else {
		if(outgoing_rear == MAX_ITEMS-1) {
			outgoing_rear = 0;
		}
		else {
			outgoing_rear = outgoing_rear + 1;
		}
	}
	c_outgoing_queue_arr[outgoing_rear] = item ;
	printf("Element inserted to queue is : %d",c_outgoing_queue_arr[outgoing_rear]);

	return true;
}

bool insert_incoming_value(uint8_t item)
{
	if ((incoming_front == 0 && incoming_rear == MAX_ITEMS - 1) || (incoming_front == incoming_rear + 1)) {
		printf("Queue Overflow");
		return false;
	}

	if (incoming_front == -1) {
		incoming_front = 0;
		incoming_rear = 0;
	} else {
		if(incoming_rear == MAX_ITEMS-1) {
			incoming_rear = 0;
		}
		else {
			incoming_rear = incoming_rear + 1;
		}
	}
	c_incoming_queue_arr[incoming_rear] = item ;
	printf("Element inserted to queue is : %d",c_incoming_queue_arr[incoming_rear]);

	return true;
}

void incoming_deletion()
{
	if(incoming_front == -1) {
		printf("Queue Underflown");
		return ;
	}
	printf("Element deleted from queue is : %d",c_incoming_queue_arr[incoming_front]);
	if(incoming_front == incoming_rear) {
		incoming_front = -1;
		incoming_rear=-1;
	} else {
		if(incoming_front == MAX_ITEMS-1) {
			incoming_front = 0;
		} else {
			incoming_front = incoming_front + 1;
		}
	}
}

void outgoing_deletion()
{
	if(outgoing_front == -1) {
		printf("Queue Underflown");
		return ;
	}
	printf("Element deleted from queue is : %d",c_outgoing_queue_arr[outgoing_front]);
	if(outgoing_front == outgoing_rear) {
		outgoing_front = -1;
		outgoing_rear=-1;
	} else {
		if(outgoing_front == MAX_ITEMS-1) {
			outgoing_front = 0;
		} else {
			outgoing_front = outgoing_front + 1;
		}
	}
}

uint8_t get_incoming_value()
{
	if(incoming_front == -1) {
		printf("Queue is empty");
		return 0;
	}

	uint8_t return_value = c_incoming_queue_arr[incoming_front];
	incoming_deletion();
	return return_value;
}

uint8_t get_outgoing_value()
{
	if(outgoing_front == -1) {
		printf("Queue is empty");
		return 0;
	}

	uint8_t return_value = c_outgoing_queue_arr[outgoing_front];
	outgoing_deletion();
	return return_value;
}
