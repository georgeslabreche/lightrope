/*
 * Title:
 * 	The command object. This object stores the command that needs to be
 *	executed into a shared variable. The variable is thread safe
 *	by use of a mutual exclusion semaphore.
 *
 * Authors:
 * 	Georges L. J. Labreche <geolab-7@student.ltu.se>
 * 	Natalie Lawton <natlaw-7@student.ltu.se>
 */
#include <FreeRTOS.h>

/* Access to predefined semaphore methods and macros. */
#include <semphr.h>

/* Header file for this object. */
#include "command.h"

static xSemaphoreHandle xSemaphore;
static unsigned char sharedCommand;

/*
 * Initialise the command object.
 */
void initCom(void)
{
	/* 
	 * Create a sempahore to manage mutex code blocks that operate
	 * on shared resourced. In this case, the command value.
	 */ 
	xSemaphore = xSemaphoreCreateMutex();
	
	/*
	 * Set default command.
	 * No need to protect this assignment with a semaphore since no 
	 * tasks are trying to access it while it is being initialised.
	 */
	sharedCommand = 0b00000000;
}

/*
 * Write the command into a shared memory variable.
 */
void writeCom(unsigned char command)
{
	/* First obtain the semaphore before executing following code. */ 
	xSemaphoreTake(xSemaphore, portMAX_DELAY);
	/* Semaphore obtained. Run code that uses the shared resource. */

	/* Write the command variable into the shared memory. */
	sharedCommand = command;

	/* Mutex operation completed, release the semaphore. */ 
	xSemaphoreGive(xSemaphore);	
}

/*
 * Read the command from a shared variable.
 */
unsigned char readCom(void)
{
	/* Variable to assign the value of the command. */
	unsigned char command;

	/* First obtain the semaphore before executing following code. */ 
	xSemaphoreTake(xSemaphore, portMAX_DELAY);
	/* Semaphore obtained. Run code that uses the shared resource. */
		
	/* Read the command from the shared memory. */
	command = sharedCommand;
		
	/* Mutex operation completed, release the semaphore. */ 
	xSemaphoreGive(xSemaphore);
 
	/* Return the read command. */
	return command;
}
