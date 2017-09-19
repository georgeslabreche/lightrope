/*
 * Title:
 * 	The lightrope object.
 *
 * Description:
 *	This object displays individual	LEDs in a sequence and
 *	speed that are determined by the command read from a
 *	shared memory variable.
 *
 *	It is in this object that the different states of the
 *	program are transitioned to and from. There are 5 states,
 *	each assigned a unique unsigned int identifier.
 *
 *    	0 - Init: All LED lights are switched on.
 *	1 - Start: All LED lights are switched off.
 *	2 - Ready: Only LED light LED0 is switched on.
 *	3 - Low: Lightrope sequence engaged at low speed.
 *	4 - High: Lightrope sequence engaged at high speed.
 *
 * Authors:
 * 	Georges L. J. Labreche <geolab-7@student.ltu.se>
 * 	Natalie Lawton <natlaw-7@student.ltu.se>
 */
#include <avr/io.h>
#include <FreeRTOS.h>

/* Access to task related methods and macros. */
#include <task.h>

/* Access to command related methods. */
#include "command.h"

/* The header of this object. */
#include "lightrope.h"

/*
 * This method is the logic run by the lightrop task.
 * It reads the command set in a shared memory value
 * and after interpreting its value, initiates the
 * expected LED display as per specifications.
 */
static void runLightrope(){
	
    /* 
     * Save the time the task was put active
     * for the last time in this variable
     */
    portTickType xLastWakeTime;

    /*
     * Initialize the variable once.
     * It will be updated automatically
     */
    xLastWakeTime = xTaskGetTickCount();

	/* Delay in loop before entering next iteration */
	uint16_t delayTime = 150;

	/* The variable that will contain the command value */
	unsigned char command = NULL;

	/* Initial state set to state 0 - Init. */
	unsigned int state = 0;
	
	/* At initial state 0 we want all LEDS switched on */ 
	PORTB = 0b00000000;

	/* Enter the task's while loop. */
	while(1)
    	{
		/* 
		 * Grab command value from the Queue. The value
		 * was set using an interrupt that invoked the
		 * writeCom method in the command object.
		 */ 
		command = readCom();

		/* Enter initial state: State 0 - Init. */
		if(state == 0)
		{
			/* 
			 * Do nothing in this state until the interrupt 
			 * command is given to go to the next state: 
			 * State 1 - Start.
			 */
			if(command == 0b0000101){
				/* 
				 * Command received to move to state 1.
				 *
				 * At State 1, all LED off. 
				 * We set that here instead of in the 
				 * state 1 condition code block to avoid
				 * continuously setting PORTB in a loop 
				 * while in State 1. Same logic applies
				 * for transition from State 1 to 2 and
				 * from 2 to 3 or 4.
				 */
				PORTB = 0b11111111;
				
				/* Set state for next loop iteration. */
				state = 1;
			}
		}
		
		/* Enter State 1 - Start. */
		else if(state == 1)
		{
			/*
			 * All LED off already set for this state.
			 * Do nothing in this state until the interrupt 
			 * command is given to go to the next state: 
			 * State 2 - Ready.
			 */
			if(command == 0b00000100)
			{
				/* 
				 * Command received to move to state 2.
				 * State 2 - Ready: only LED light LED0
				 * is switched on.
				 */
				PORTB = 0b11111110;
				
				/* Set state for next loop iteration. */
				state = 2;

			}

		}
		
		/* Enter State 2 - Ready. */
		else if(state == 2)
		{
			/**
			 * Only LED light LED0 switched already set
			 * for this state. Do nothing in this state
			 * until the interrupt command is given to 
			 * go to one of the two possible next states: 
			 * State 3 - Low or State 4: High.
			 */
			if(command == 0b00000101)
			{
				/* 
				 * Transition to state Low selected.
				 * Set state for next loop iteration.
				 */
				state = 3;

			}
			else if(command == 0b00000110)
			{
				/* 
				 * Transition to state High selected.
				 * Set state for next loop iteration.
				 */
				state = 4;
			}

		}
		/*
		 * Enter either state 3 - Low, or state 4 - High.
		 * The only difference between both states is the
		 * speed of the lightrope sequence as determined by
		 * the value set as the loop delay.
		 */
		else if(state == 3 || state == 4) {
		    /* 
		     * Light the next LED (shift a bit)
		     * Use "~" to invert as 0 is on and 1 is off
		     * e.g. 0b11111110 --> 0b11111101
		     */
		    PORTB = ~((~PINB)<<1);

		    /* If the lightrope reaches the end, start again. */
		    if(PINB == 0xFF)
		        PORTB = ~1;

			/*
			 * Determine which lightrope sequence to engage
			 * based on the command. Alternatively, could have
			 * look the the state id as well.
			 */
			switch (command)
			{
				/* If command is LOW speed. */
				case (0b00000101):
				{
					delayTime = 300;
					break;
				}
				/* Else if command is HIGH speed. */
				case (0b00000110):
				{
					delayTime = 10;
					break;
				}
				default: break;
			}

		}

		/* Apply delay before entering next iteration of the loop */
		vTaskDelayUntil(&xLastWakeTime, ( delayTime / portTICK_RATE_MS));

	}	
}

/*
 * Initialise the lightrope object.
 */
void initLightrope(){
	/* Create aund run the lightrope task. */
	xTaskCreate(runLightrope, "runLightrope", 300, NULL, 2, NULL);
}

