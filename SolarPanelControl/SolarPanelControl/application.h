/*
 * application.h
 *
 * Created: 29/04/2024 10.51.12
 *  Author: himal
 */ 

#ifndef APPLICATION_H_
#define APPLICATION_H_

extern volatile uint8_t system_run_flag;

void initSystem();
void modifyTLow();
void modifyTHigh();
void runSystem();
void processTemp();

#endif /* APPLICATION_H_ */
