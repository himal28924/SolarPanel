/*
 * maxtrixKeyboard.h
 *
 * Created: 22/04/2024 10.53.50
 *  Author: himal
 */ 


#ifndef MAXTRIXKEYBOARD_H_
#define MAXTRIXKEYBOARD_H_
#include <avr/io.h>

extern volatile int matrixValue;
void init_matrix_keyboard();
int8_t getxKey();



#endif /* MAXTRIXKEYBOARD_H_ */