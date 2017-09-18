#include <avr/io.h>
#include <FreeRTOS.h>
#include <task.h>
#include "task2.h"
#include "diode.h"

static void blinkTask2(){

	// Save the time the task was put active
    // for the last time in this variable
    portTickType xLastWakeTime;

    // Initialize the variable once
    // It will be updated automatically
    xLastWakeTime = xTaskGetTickCount();

	while(1)
    {
		toggleDiode(0b0000010);
		vTaskDelayUntil(&xLastWakeTime, ( 100 / portTICK_RATE_MS));
	}
}

void initTask2(){
	//PORTB = 0b11111101;
	xTaskCreate(blinkTask2, "blinkTask2", 300, NULL, 1, NULL);
}

