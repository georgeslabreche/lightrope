#include <avr/io.h>
#include <FreeRTOS.h>
#include "task1.h"
#include "task2.h"



void initPins(){
	DDRB  = 0xFF; // Initialize PORTB to output
	DDRD  = 0x00; // Initialize PORTD as input
    PORTD = 0xFF; // Activate pull-ups
	PORTB = 0b11111110;
}


int main(void)
{
	initPins();
	initDiode();
	initTask1();
	initTask2();

    vTaskStartScheduler();

    return 0;
}
