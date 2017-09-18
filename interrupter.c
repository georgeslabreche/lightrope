#include <avr/io.h>
#include <FreeRTOS.h>
#include <queue.h>
#include "interrupter.h"

void initInterrupt(){
	EICRA = 0b00000000;
	EIMSK = 0b00000100;

	xQueueSendToBackFromISR();
}
