/*
 * displayWithSPI.c
 *
 * Created: 20/06/2024 12.50.02
 *  Author: himal
 */

#include <avr/io.h>
#include "displayDriver.h"
#include "avr/interrupt.h"

#define REFRESH_INTERVAL_MS 4
#define F_CPU 16000000UL

// Define control pins
#define SS_PIN   PB0  // Slave Select pin for SPI
#define SHCP_PIN PB1  // Shift register clock pin (Now unused in SPI mode)
#define STCP_PIN PB0  // Storage register clock pin (Latch pin)

#define DIGIT1_PIN PF0
#define DIGIT2_PIN PF1
#define DIGIT3_PIN PF2
#define DIGIT4_PIN PF3

// Define segment maps here
const uint8_t segmentMap[10] = {
    0b11111100, // 0
    0b01100000, // 1
    0b11011010, // 2
    0b11110010, // 3
    0b01100110, // 4
    0b10110110, // 5
    0b10111110, // 6
    0b11100000, // 7
    0b11111110, // 8
    0b11110110  // 9
};

const uint8_t segmentMapWithDP[10] = {
    0b11111101, // 0
    0b01100001, // 1
    0b11011011, // 2
    0b11110011, // 3
    0b01100111, // 4
    0b10110111, // 5
    0b10111111, // 6
    0b11100001, // 7
    0b11111111, // 8
    0b11110111  // 9
};

void init_display(void) {
    // Initialize data direction registers for control pins
    DDRB |= (1 << SS_PIN) | (1 << PB2) | (1 << PB1);  // SS, MOSI, and SCK as outputs
    DDRF |= (1 << DIGIT1_PIN) | (1 << DIGIT2_PIN) | (1 << DIGIT3_PIN) | (1 << DIGIT4_PIN);

    // Initialize SPI
    SPCR |= (1 << SPE) | (1 << MSTR); // Enable SPI, Master mode
    SPCR &= ~((1 << DORD) | (1 << CPOL) | (1 << CPHA)); // MSB first, leading edge sampling
    SPSR |= (1 << SPI2X); // Double speed mode

    // Initializing timer for multiplexing
    TCCR1A = 0x00;
    TCCR1B = (1 << WGM12) | (1 << CS10) | (1 << CS11); // CTC mode, prescaler 64
    OCR1A = (F_CPU / 64 / 1000) * REFRESH_INTERVAL_MS - 1;
    TIMSK1 |= (1 << OCIE1A); // Enable compare match interrupt
}

void display_float(float number, bool isFloat) {
    int intPart = (int)number;
    int decimalPart = (int)((number - intPart) * 100);

    digits[0] = intPart / 10;  // Tens
    digits[1] = intPart % 10;  // Ones
    digits[2] = decimalPart / 10;  // Tenths
    digits[3] = decimalPart % 10;  // Hundredths

    segment_data[0] = segmentMap[digits[0]];
    if (isFloat)
    {
        segment_data[1] = segmentMapWithDP[digits[1]];  // Decimal after this digit
    } else {
        segment_data[1] = segmentMap[digits[1]];
    }
    segment_data[2] = segmentMap[digits[2]];
    segment_data[3] = segmentMap[digits[3]];
}

void display_digit(uint8_t digit) {
    uint8_t segments = segment_data[digit];

    // Activate the correct digit
    activate_digit(digit);

    // Send the segment data via SPI
    PORTB &= ~(1 << SS_PIN); // Select the shift register
    SPDR = segments; // Load data into SPI data register
    while (!(SPSR & (1 << SPIF))); // Wait for transmission to complete
    PORTB |= (1 << SS_PIN); // Deselect the shift register

    // Latch the data onto the outputs
    toggle_stcp();
}

void activate_digit(uint8_t digit) {
    PORTF |= (1 << DIGIT1_PIN) | (1 << DIGIT2_PIN) | (1 << DIGIT3_PIN) | (1 << DIGIT4_PIN);
    PORTF &= ~(1 << (DIGIT1_PIN + digit));  // Activate the selected digit
}

void toggle_stcp(void) {
    PORTB |= (1 << STCP_PIN);  // Set STCP_PIN high
    PORTB &= ~(1 << STCP_PIN); // Set STCP_PIN low
}

ISR(TIMER1_COMPA_vect) {
    static uint8_t current_digit = 0;
    display_digit(current_digit);  // Display current digit
    current_digit = (current_digit + 1) % 4;  // Cycle through digits
}
