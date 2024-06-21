/*
 * displayDriver.h
 *
 * Created: 15/04/2024 09.47.32
 *  Author: himal
 */ 
#ifndef DISPLAYDRIVER_H_
#define DISPLAYDRIVER_H_
#include <stdbool.h>
// Define pins used
#define SHCP_PIN PB1  // Shift register clock pin
#define STCP_PIN PB0  // Storage register clock pin
#define DS_PIN   PB2  // Data pin for the shift register

#define DIGIT1_PIN  PF0
#define DIGIT2_PIN  PF1
#define DIGIT3_PIN  PF2
#define DIGIT4_PIN  PF3

extern const uint8_t segmentMap[10];  
extern const uint8_t segmentMapWithDP[10];  

volatile uint8_t digits[4];  
uint8_t segment_data[4];  // Holds the segment data for each digit

void init_display();
void display_digit(uint8_t digit);
void activate_digit(uint8_t digit);
void toggle_shcp();
void toggle_stcp();
void display_float(float number, bool isFloat);

#endif /* DISPLAYDRIVER_H_ */
