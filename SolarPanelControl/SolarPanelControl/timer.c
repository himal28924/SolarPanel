/*
 * timer.c
 *
 * Created: 19/04/2024 11.15.55
 *  Author: himal
 */ 
#include "avr/io.h"
#include <avr/interrupt.h>

void init_timer() {
	// Reset Timer/Counter Control Registers for Timer 4
	TCCR4A = 0;
	TCCR4B = 0;
	
	// Set the timer to CTC (Clear Timer on Compare Match) Mode
	// WGM42 bit = 1 for CTC mode on Timer 4
	TCCR4B |= _BV(WGM42);
	TCCR4A |= _BV(COM4A0);
	
	// Set prescaler to 1024 and start the timer
	TCCR4B |= _BV(CS42) | _BV(CS40)	;
	
	// Calculate and set OCR4A for a 1-second interval at 16 MHz clock with 1024 prescaler
	OCR4A = 15624;
	
	// Enable the Output Compare Match A Interrupt for Timer 4
	TIMSK4 |= _BV(OCIE4A);
}
