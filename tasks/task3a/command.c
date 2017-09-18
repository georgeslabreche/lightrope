#include <FreeRTOS.h>
#include <semphr.h>
#include "command.h"

static xSemaphoreHandle xSemaphore ;
static unsigned char sharedCommand;

void initCom(void)
{
	xSemaphore = xSemaphoreCreateMutex();
	sharedCommand = 0b00000000;
}

void writeCom(unsigned char command)
{
	// See if we can obtain the semaphore. 
	xSemaphoreTake(xSemaphore, portMAX_DELAY);
   
	// Semaphore obtained.
	// Run code that uses the shared resource.
	
	// Write in shared memory
	sharedCommand = command;
	
	// Finished accessing the shared resource .
	// Release the semaphore.
	xSemaphoreGive(xSemaphore);
	
}

unsigned char readCom(void)
{
	unsigned char command;

	xSemaphoreTake(xSemaphore, portMAX_DELAY);
	// Semaphore obtained.
	// Run code that uses the shared resource.
		
	// Read from shared memory
	command = sharedCommand;
		
	// Finished accessing the shared resource .
	// Release the semaphore.
	xSemaphoreGive(xSemaphore);
 

	return command;
}
