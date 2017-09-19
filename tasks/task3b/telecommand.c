/*
 * Title:
 * 	The telecommand object.
 *
 * Description:
 *	This object relays messages between the command
 *	object and the lightrope object. It uses interrupts
 *	to dispatch a message that is then saved in a shared
 *	memory variable in the command object.
 *
 * Authors:
 * 	Georges L. J. Labreche <geolab-7@student.ltu.se>
 * 	Natalie Lawton <natlaw-7@student.ltu.se>
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include "command.h"
#include "telecommand.h"

/* The message queue used to relay commands. */
xQueueHandle xQueue = NULL;

/*
 * The telecommand task method.
 * Dequeues commands from the queue and writes them
 * into the shared memory variable in the command object.
 * Commands are enqueued into the queue from the interrupt
 * service routine.
 */
static void listenTC()
{	
	/* 
	 * Save the time the task was put active
	 * for the last time in this variable.
	 */
    	portTickType xLastWakeTime;
	
	/* 
	 * Initialize the variable once.
	 * It will be updated automatically.
	 */
	xLastWakeTime = xTaskGetTickCount();

	/*
	 * The command value will be read from the queue
	 * and assigned into this command variable. */
	unsigned char command = NULL;

	/* Enter the task loop. */
	while(1)
    	{
		/* 
		 * Dequeue command sent from the interrupt service
		 * routine.
		 */
		xQueueReceive(xQueue, &(command), portMAX_DELAY);

		/* 
		 * Write dequeued command into the shared memory variable
		 * so that it is later read by the lightrope object.
		 */
		writeCom(command);
		
		/* Time delay before next loop iteration. */
		vTaskDelayUntil(&xLastWakeTime, (100 / portTICK_RATE_MS));
	}	
}

/*
 * Initalise the telecommand object.
 */
void initTC()
{
	/* Create the interrupt message relay queue. */
	xQueue = xQueueCreate(1, sizeof(unsigned char));
	
	/* Create and run the telecommand task. */
	xTaskCreate(listenTC, "listenTC", 300, NULL, 1, NULL);
}

/*
 * Initalise the interrupt registry.
 */
void initInterrupt()
{
	/* Use pull up as input, so no need to set EICRA. */
	/* Using external interrupts INT0 as interrupt trigger */
	EIMSK = (1 << INT0);
	
	/* Activate interrupts */
	sei();
}

/*
 * Interrupt service routing for INT0.
 */
ISR(INT0_vect)
{
	/* Grap inverse value of PIND when interrupt trigger is fired. */
	unsigned char button = ~PIND;
	
	/* And enqueue it in the command relay queue. */
	xQueueSendToBackFromISR(xQueue, &button, NULL);
}
