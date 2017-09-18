#include <avr/io.h>
#include <FreeRTOS.h>
#include <queue.h>
#include "interrupter.h"

void initInterrupt(){
	// Use pull up as input, so don't set EICRA.
	EIMSK = (1 << INT0); // activate INT0 interrupt
	sei(); //activate interrupts
	xQueueSendToBackFromISR();
}
