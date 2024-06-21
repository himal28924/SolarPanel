/*
 * matrxiKeyboard.c
 *
 * Created: 22/04/2024 10.54.41
 *  Author: himal
 */ 

#include "maxtrixKeyboard.h"
volatile int matrixValue = 0;

typedef struct matrixKeyboard_descr_t{
	volatile uint8_t* ddr;
	volatile uint8_t* pin;
	volatile uint8_t* port;
	uint8_t bit;
} matrixKeyboard_descr_t;

// Led register addresses in I/O map

static matrixKeyboard_descr_t keys_row[] = {
	{&DDRB, &PINB, &PORTB, PB7},
	{&DDRB, &PINB, &PORTB, PB5},
	{&DDRB, &PINB, &PORTB, PB4},
	{&DDRE, &PINE, &PORTE, PE3},
};


static matrixKeyboard_descr_t keys_col[] = {
	{&DDRE, &PINE, &PORTE, PE5},
	{&DDRH, &PINH, &PORTH, PH3},
	{&DDRH, &PINH, &PORTH, PH5},
	{&DDRL, &PINL, &PORTL, PL1},
};

void init_matrix_keyboard(){
	for(uint8_t i = 0; i < 4; i++)
	{
		// setting row as output 
		*(keys_row[i].ddr) |= _BV(keys_row[i].bit);
		*(keys_row[i].port) &= ~_BV(keys_row[i].bit);
		
		 *(keys_col[i].ddr) &= ~_BV(keys_col[i].bit);  // Set column pin as input
		 *(keys_col[i].port) |= _BV(keys_col[i].bit); // Enable pull-up resistor
	}
}

int8_t getxKey(){
	for (uint8_t i = 0; i < 4; i++) {
		*(keys_row[i].port) |= _BV(keys_row[i].bit);  // Set all rows high before scanning
	}

	for (uint8_t row = 0; row < 4; row++)
	{
		// setting row as output by setting it low 
		*(keys_row[row].port) &= ~_BV(keys_row[row].bit);
		
		for (uint8_t col = 0; col < 4; col++)
		{
			// Checking if pin is low or not 
			if ((*(keys_col[col].pin) & _BV(keys_col[col].bit))==0)
			{
				matrixValue = row *10 + col;
				return matrixValue;// so now if row is 2 and col is 3 it should retun 23
			}
		}
		// before leaving setting row to 1
		*(keys_row[row].port) |= _BV(keys_row[row].bit);
	}
	return 255;
}