/*
 * application.c
 *
 * Created: 29/04/2024 10.51.52
 *  Author: himal
 */ 

#include <avr/interrupt.h>
#include <avr/delay.h>
#include "application.h"
#include "drivers/thermometer/thermometer_driver.h"
#include "drivers/7segmentDisplay/displayDriver.h"
#include "timer.h"
#include "drivers/matrxiKeyboard/maxtrixKeyboard.h"
#include "drivers/led/led_driver.h"
#include "drivers/keys/key_input_driver.h"

volatile uint8_t system_run_flag = 0;
float tLow = 0.00;
float tHigh = 25.00;

uint8_t user_is_modifing = 0;

void initSystem(){
    init_matrix_keyboard();
    init_leds();
    init_thermometer();
    init_display();
    init_timer();
    display_float(0.0f, false);
    init_keys();
}

void runSystem(){
    if (get_key(1)){
        user_is_modifing = 1;
        set_led(1, 1); // Turn on LED 1
        set_led(2, 0); // Turn off LED 2
        set_led(3, 0); // Turn off LED 3
        _delay_ms(100);
        modifyTLow();
        set_led(1, 0);
        user_is_modifing = 0;
    } else if (get_key(2)){
        user_is_modifing = 1;
        set_led(1, 0); // Turn off LED 1
        set_led(2, 1); // Turn on LED 2
        set_led(3, 0); // Turn off LED 3
        _delay_ms(100);
        modifyTHigh();
        set_led(2, 0);
        user_is_modifing = 0;
    } else if (get_key(3)){
        set_led(1, 0); // Turn off LED 1
        set_led(2, 0); // Turn off LED 2
        set_led(3, 1); // Turn on LED 3
        processTemp();
    } else if (user_is_modifing == 0){
        processTemp();
    }
    system_run_flag = 0;
}

uint8_t mapKeyToDigit(uint8_t key) {
	switch(key) {
		case 0: return 1;  // 00 -> 1
		case 1: return 2;  // 01 -> 2
		case 2: return 3;  // 02 -> 3
		case 10: return 4; // 10 -> 4
		case 11: return 5; // 11 -> 5
		case 12: return 6; // 12 -> 6
		case 20: return 7; // 20 -> 7
		case 21: return 8; // 21 -> 8
		case 22: return 9; // 22 -> 9
		case 31: return 0; // 30 -> 0
		case 30: return 30; // 31 -> *
		case 32: return 32; // 32 -> #
		default: return 255; // Invalid key
	}
}


void processTemp(){
	float temperature  = 0.00;
	getTemperatureInCelsius();
	temperature  = temperatureInCelsius;
    display_float(temperature, true);

    if (temperature < tLow) {
        set_led(7, 0); // Turn off pump
        set_led(6, 1);  // Open valve
    } else if (temperature > tHigh) {
        set_led(7, 1);  // Turn on pump
        set_led(6, 0); // Close valve
    } else {
        // Normal operating range
        set_led(6, 0); // Valve closed
        set_led(7, 0);  // Pump closed
    }
}
void modifyTLow() {
	display_float(tLow, false);
	_delay_ms(100);
	float new_value = 0;
	uint8_t digit_count = 0;
	uint8_t key = 255;
	while (1) {
		key = getxKey();
		if (key == 32) { // '#' key
			if (digit_count > 0) {
				tLow = new_value;
				display_float(tLow, true);
			}
			break;
			} else if (key == 30) { // '*' key
			// Cancel the operation, restore previous value
			display_float(tLow, false);
			break;
			} else if (key != 255) { // Digit keys 0-9
				digit_count++;
				if (digit_count == 3){
					new_value = 0;
					digit_count = 1;
				}
				uint8_t digit = mapKeyToDigit(key);
				key = 255;
				new_value = new_value * 10 + digit;
				display_float(new_value, false);
		}
		_delay_ms(100); // Small delay to avoid bouncing issues
	}
}

void modifyTHigh() {
	display_float(tHigh, true);
	_delay_ms(100);
	float new_value = 0;
	uint8_t digit_count = 0;

	while (1) {
		uint8_t key = getxKey();
		uint8_t digit = mapKeyToDigit(key);
		if (digit == 32) { // '#' key
			if (digit_count > 0) {
				tHigh = new_value;
				display_float(tHigh, true);
			}
			break;
			} else if (digit == 30) { // '*' key
			// Cancel the operation, restore previous value
			display_float(tHigh, false);
			break;
			} else if (digit <= 9) { // Digit keys 0-9
			new_value = new_value * 10 + digit;
			display_float(new_value, true);
			digit_count++;
		}
		_delay_ms(100); // Small delay to avoid bouncing issues
	}
}



ISR(TIMER4_COMPA_vect) {
    // This ISR will be called approximately every 1 second
    system_run_flag = 1;
}
