/*
 * key_input_driver.h
 *
 * Created: 19/02/2024 11.35.49
 *  Author: himal
 */ 


#ifndef KEY_INPUT_DRIVER_H_
#define KEY_INPUT_DRIVER_H_

#include <stdint.h> 
#include <stdbool.h>

#define KEY_PORT_ALL_INPUT 0x00 ;
#define KEY_ALL_INPUT_ENABLE_PULL_UP_RESISTORS 0Xff ;

void init_keys();
bool get_key(uint8_t key_no); // key_no [1..6], return true if key pressed



#endif /* KEY_INPUT_DRIVER_H_ */