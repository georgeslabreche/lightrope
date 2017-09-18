#include <avr/io.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "task2.h"

xSemaphoreHandle xSemaphore = NULL;

void initDiode(){
	xSemaphore = xSemaphoreCreateMutex();
}

void toggleDiode(unsigned char ledPattern)
{
	// Only run code if sempahore has been initalised.
	if(xSemaphore != NULL){
		// See if we can obtain the semaphore. If not available, wait 10 ticks
		// before trying again.
		if(xSemaphoreTake(xSemaphore, (portTickType) 10) == pdTRUE){
			// Semaphore obtained.
			// Run code that uses the shared resource (PORTB).
			PORTB = PORTB ^ ledPattern;
			
			// Finished accessing the shared resource (PORTB).
			// Release the semaphore.
			xSemaphoreGive(xSemaphore);
		}
		else{
			// Semaphore not available.
			// Try again on the next iteration.
		}
	
	}
}
