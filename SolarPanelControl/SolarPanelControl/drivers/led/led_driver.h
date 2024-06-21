/*
 * led_driver.h
 *
 * Created: 19/02/2024 11.24.17
 *  Author: himal
 */ 


#ifndef LED_DRIVER_H_
#define LED_DRIVER_H_

#define LED_PORT_All_OUTPUT 0xff;
#define LED_ALL_OFF 0xff;

#include <stdint.h>
#include <stdbool.h>

void init_leds();
void set_led(uint8_t led_no, uint8_t state);
bool isvalidRange(uint8_t led_no); 

#endif /* LED_DRIVER_H_ */