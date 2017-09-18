#include <avr/io.h>
#include <FreeRTOS.h>
#include <queue.h>
#include "telecommandb.h"

QueueHandle xQueue;


static void listenTC(){
	
	unsigned char *command;

	// Receive a command message on the create queue.
	xQueueReceive(xQueue, &(command), portMAX_DELAY)
	writeCom(command);	
}


void initTC(){
	xQueue = xQueueCreate(3, sizeof(unsigned char));
}

