#include <avr/io.h>
#include <FreeRTOS.h>
#include "command.h"
#include "lightrope.h"
#include "telecommand.h"


void initPins(){
	DDRB  = 0xFF; // Initialize PORTB to output
	DDRD  = 0x00; // Initialize PORTD as input
    PORTD = 0xFF; // Activate pull-ups
	PORTB = 0b11111110;
}


int main(void)
{
	initPins();
	//initDiode();
	initCom();
	initLightrope();
	initTC();

    vTaskStartScheduler();

    return 0;
}
