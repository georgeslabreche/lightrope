/*
 * Title: Assignment 2 - Tasks in FreeRTOS 
 *
 * Reason:
 * 	- To familiarise with the task concept.
 *	- To familiarise with the FreeRTOS.
 *	- To familiarise with the STK500 development board.
 *	- To prepare for the final RTOS system lab.
 *
 * Authors:
 * 	Georges L. J. Labreche <geolab-7@student.ltu.se>
 *	Natalie Lawton <natlaw-7@student.ltu.se>
 */
#include <avr/io.h>
#include <FreeRTOS.h>

/* Access to command related methods. */
#include "command.h"

/* Access to lightrope related methods. */
#include "lightrope.h"

/* Access to telecommand related methods. */
#include "telecommand.h"

/*
 * Initalise pins and ports that will be used.
 */
static void initPins(){
	DDRB  = 0xFF; /* Initialise PORTB to output. */
	DDRD  = 0x00; /* Initialise PORTD as input. */
    	PORTD = 0xFF; /* Activate pull-ups. */
	PORTB = 0b11111110; 
}

/*
 * Main progran execution block.
 */
int main(void)
{
	initPins(); /* Initialise pins. */
	initInterrupt(); /* Initialise interrupt in telecommand object. */
	initCom(); /* Initialise command object. */
	initLightrope(); /* Initialise lightrope object. */
	initTC(); /* Initialise telecommand. */
	
	vTaskStartScheduler(); /* Start the scheduler to manage tasks. */

	return 0;
}
