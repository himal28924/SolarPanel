/*
 * led_driver.c
 *
 * Created: 19/02/2024 11.24.29
 *  Author: himal
 */ 
#include <avr/io.h>
#include "led_driver.h"

void init_leds() {
	DDRA = LED_PORT_All_OUTPUT;
	PORTA = LED_ALL_OFF;
}

void set_led(uint8_t led_no, uint8_t state) {
    if (isvalidRange(led_no)) {
        // To make sure led starts from 1 
        led_no--;
        if (state == 1) {
            //Since, Leds are active low , setting led to 0/clear bit 
            PORTA &= ~(1 << led_no);
        }
        else {
            // Set bit to turn LED off
            PORTA |= (1 << led_no); 
        }
    }
}

// Making sure that passed led no is with in the range 
bool isvalidRange(uint8_t led_no) {
	if (led_no >= 1 && led_no <= 8) {
		return true;
	}
	return false;
}