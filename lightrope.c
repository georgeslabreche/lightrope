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

	uint16_t delayTime = 100; // was 500
	PORTB=~1;

    // 0 - init
	// 1 - start
	// 2 - ready
	// 3 - low
	// 4 - high
	unsigned int state = 0;
	unsigned char command;

	PORTB = 0b00000000;

	while(1)
    {
		// Grab command data from Queue
		command = readCom();

		if(state == 0)
		{
			if(command == 0b0000101){
				PORTB = 0b11111111;
				state = 1;
			}

		}else if(state == 1)
		{
			if(command == 0b00000100)
			{
				PORTB = 0b11111110;
				state = 2;

			}

		}else if(state == 2)
		{
			if(command == 0b00000101)
			{
				state = 3;

			}else if(command == 0b00000110)
			{
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
				// If command is LOW frequency	
				case (0b00000101):
				{
					delayTime = 300;
					break;
				}
				// Else if command is HIGH frequency
				case (0b00000110):
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

