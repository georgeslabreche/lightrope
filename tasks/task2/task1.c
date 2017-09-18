#include <avr/io.h>
#include <FreeRTOS.h>
#include <task.h>
#include "task1.h"
#include "diode.h"

static void blinkTask1(){
	
	// Save the time the task was put active
    // for the last time in this variable
    portTickType xLastWakeTime;

    // Initialize the variable once
    // It will be updated automatically
    xLastWakeTime = xTaskGetTickCount();

	while(1)
    {
		toggleDiode(0b0000001);
		vTaskDelayUntil(&xLastWakeTime, ( 250 / portTICK_RATE_MS));
	}	
}

void initTask1(){
	//PORTB = 0b11111110;
	xTaskCreate(blinkTask1, "blinkTask1", 300, NULL, 1, NULL);
}

