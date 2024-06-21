/*
 * key_input_driver.c
 *
 * Created: 19/02/2024 11.36.07
 *  Author: himal
 */ 

#include "key_input_driver.h"
#include <avr/io.h>

void init_keys() {
	DDRC = 0x00;
	PORTC = 0xff;
}

// key_no [1..6], return true if key pressed
bool get_key(uint8_t key_no) {
    if (key_no < 1 || key_no > 6) {
        return false; // Invalid key number
    }
    return !(PINC & (1 << (key_no - 1))); //  0 if the key is pressed (active-low).
}