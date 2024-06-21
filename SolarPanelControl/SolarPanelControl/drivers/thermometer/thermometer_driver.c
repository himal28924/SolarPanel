/*
 * thermometer_driver.c
 *
 * Created: 11/03/2024 10.47.00
 *  Author: himal
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "thermometer_driver.h"

// 
static uint8_t valueFromTempSensor;
volatile float temperatureInCelsius = 0.00;
// Define the pin and port for TMP36 enable
#define TMP36_ENABLE_PIN PG0
#define TMP36_ENABLE_PORT PORTG
#define TMP36_ENABLE_DDR DDRG

// Converting values defination
#define VREF 5.0 // Reference voltage
#define ADC_RESOLUTION 256.0 // For 8-bit result

void init_thermometer_enable_pin() {
	// Set PG0 as an output
	TMP36_ENABLE_DDR |= _BV(TMP36_ENABLE_PIN);

	// Set PG0 to hizgh to enable the TMP36
	TMP36_ENABLE_PORT |= _BV(TMP36_ENABLE_PIN);
}



void init_thermometer(){
	// Select input pin as shown in data sheet  TMP36-VOUT -- ADC15 -- PK7 
	DDRK &= ~_BV(PK7);
	
	init_thermometer_enable_pin();
	
	// Setting AVC as reference - REFS0 
	// Set mux5 According to data sheet ADC15 is mux 5 is : 100111 so mu0 mu1 mu2 mu5 this are analog input pin 
	// Left adjust result as only 8-bit precision is needed . - ADLAR -- Now we can read result from ADCH 
	
	// Input pin of ADC 15 pk7 <--Read voltage level   --- ADC ------ Convert it to digital based on REFS0 , AVCC and resolution setting --> ADC data register ADCH (*value is ready to read)
	ADMUX |= _BV(REFS0) | _BV(ADLAR) | _BV(MUX0) | _BV(MUX1) | _BV(MUX2);

	// since admux can not set mux5 we need to set it from adc control and status register b 
	ADCSRB |= _BV(MUX5);
	
	// Now enabling ADC and configuration for free running mode with interrupts 
	// ADCSRA - ADC control and status register A : controls various aspect of ADC , i.e enable ADC , start conversion, enable interrupt , set ADC clock speed 
	// ADEN - Enable ADC - anolug to digital 
	// ADATE - Auto trigger 
	// ADIE - ADC interrupt , when adc conversion is complete interrupt is triggered . It allows program to perform other task 
	// ADPS2 - ADC preseclar selection .. select adc speed and set the speed to 1/16th of micro controller , 1MHZ 
	
	// Turn on ADC , configure it to continuously convert analog to digital and trigger interrupt once done  and set adc speed 
	ADCSRA |= _BV(ADEN) | _BV(ADATE) | _BV(ADIE) | _BV(ADPS2);
	
	//BUt for our timer to run i donot want adie 
	//ADCSRA |= _BV(ADEN) | _BV(ADATE) | _BV(ADPS2);
	// NOW  turn on the adc 
	ADCSRA |= _BV(ADSC);
	
}

// Interrupt done by adc 
ISR(ADC_vect){
	processTemperatureSensorReading();
}

// To test it 
void processTemperatureSensorReading() {
	// Encapsulated logic from ISR
	valueFromTempSensor = ADCH;
}

uint8_t getValueFromSensor(){
	uint8_t valueFromSensor;
	
	// Disable ADC interrupt while reading the shared resource valueFromTempSensor
	ADCSRA &= ~_BV(ADIE);
	valueFromSensor = valueFromTempSensor;
	
	// Re-enable ADC interrupts
	ADCSRA |= _BV(ADIE);
	return valueFromSensor;
}

void getTemperatureInCelsius() {
	uint8_t valueFromSensor = valueFromTempSensor;
	float temp;

	// Convert result from ADCH TO corresponding voltage , since we are using 5v then Vout = (valueFromTempSensor /ADC_RESOLUTION)* VREF
	float vout = (valueFromSensor/ADC_RESOLUTION)*5;
	
	// The output voltage of the sensor is 10mV/°C, and typically 750mV@25°C.
	// Temp = {(Vout - 500 mv )/10mv
	temp = ((vout-0.5)/0.01) ;
	temperatureInCelsius = temp;
}
