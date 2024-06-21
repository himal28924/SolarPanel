/*
 * main.c
 *
 * Created: 29/04/2024 09.56.41
 *  Author: himal
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "avr/interrupt.h"
#include "application.h"

int main(void)
{
	initSystem();
	sei();
	while (1)
	{
		if (system_run_flag)
		{
			runSystem();
		}
	}
}
