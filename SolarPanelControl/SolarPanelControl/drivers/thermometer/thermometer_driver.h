/*
 * thermometer_driver.h
 *
 * Created: 11/03/2024 10.46.50
 *  Author: himal
 */ 


#ifndef THERMOMETER_DRIVER_H_
#define THERMOMETER_DRIVER_H_
#include <stdint.h>

extern volatile float temperatureInCelsius;

void init_thermometer();
void processTemperatureSensorReading();
uint8_t getValueFromSensor();
void getTemperatureInCelsius();

#endif /* THERMOMETER_DRIVER_H_ */