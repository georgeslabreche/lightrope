#include <avr/io.h>
#include <FreeRTOS.h>
#include <task.h>
#include "lightrope.h"
#include "command.h"


static void runLightrope(){
	
	// Save the time the task was put active
    // for the last time in this variable
    portTickType xLastWakeTime;

    // Initialize the variable once
    // It will be updated automatically
    xLastWakeTime = xTaskGetTickCount();

	uint16_t delayTime = 500;
	PORTB=~1;

	// 1 - start
	// 2 - ready
	// 3 - low
	// 4 - high
	unsigned int state = 1;
	unsigned char command;

	while(1)
    {
		// Grab command data from shared memory
		command = readCom();

		if(state == 1){
			PORTB = 0b11111111;

			if(command == 0b11111110){
				state = 2;
			}
			
		}else if(state == 2){
			PORTB = 0b11111110;

			if(command == 0b11111110){
				state = 3;
			}
			else if(command == 0b11111101){
				state = 4;
			}

		}else if(state == 3 || state == 4) {
			// Light the next LED (shift a bit)
		    // Use "~" to invert as 0 is on and 1 is off
		    // e.g. 0b11111110 --> 0b11111101
		    PORTB = ~( (~PINB)<<1);

		    // If the lightrope reaches the end, start again
		    if(PINB == 0xFF)
		        PORTB = ~1;

			switch (command)
			{
				// If command is HIGH frequency	
				case (0b11111110):
				{
					delayTime = 300;
					break;
				}
				// Else if command is LOW frequency
				case (0b11111101):
				{
					delayTime = 10;
					break;
				}
				default: break;
			}

		}

		vTaskDelayUntil(&xLastWakeTime, ( delayTime / portTICK_RATE_MS));
	}	
}

void initLightrope(){
	xTaskCreate(runLightrope, "runLightrope", 300, NULL, 2, NULL);
}

