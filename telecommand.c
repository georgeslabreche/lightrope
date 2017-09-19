#include <avr/io.h>
#include <avr/interrupt.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include "command.h"
#include "telecommand.h"

xQueueHandle xQueue = NULL;

static void listenTC()
{
	
	// Save the time the task was put active
    // for the last time in this variable
    portTickType xLastWakeTime;

    // Initialize the variable once
    // It will be updated automatically
    xLastWakeTime = xTaskGetTickCount();

	unsigned char command = NULL;

	while(1)
    {
		xQueueReceive(xQueue, &(command), portMAX_DELAY);

		// Grab command data from PORTD;
		// Save it in the shared memory;
		// was 250
		writeCom(command);
		vTaskDelayUntil(&xLastWakeTime, (50 / portTICK_RATE_MS));
	}	
}

void initTC()
{
	xQueue = xQueueCreate(3, sizeof(unsigned char));
	xTaskCreate(listenTC, "listenTC", 300, NULL, 1, NULL);
}

void initInterrupt()
{
	// Use pull up as input, so don't set EICRA.
	// We are going to use the external interrupts INT0
	EIMSK = (1 << INT0);
	sei(); // activate interrupts
}


// Interrupt service routing for INT0
ISR(INT0_vect)
{
	//PORTB = 0b10101010;
	unsigned char button = ~PIND;
	xQueueSendToBackFromISR(xQueue, &button, NULL);
}





