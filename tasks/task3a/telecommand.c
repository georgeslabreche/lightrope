#include <avr/io.h>
#include <FreeRTOS.h>
#include <task.h>
#include "telecommand.h"
#include "command.h"


static void listenTC(){
	
	// Save the time the task was put active
    // for the last time in this variable
    portTickType xLastWakeTime;

    // Initialize the variable once
    // It will be updated automatically
    xLastWakeTime = xTaskGetTickCount();

	while(1)
    {
		// Grab command data from PORTD;
		// Save it in the shared memory;
		writeCom(PIND);
		vTaskDelayUntil(&xLastWakeTime, (250 / portTICK_RATE_MS));
	}	
}

void initTC(){
	xTaskCreate(listenTC, "listenTC", 300, NULL, 1, NULL);
}

