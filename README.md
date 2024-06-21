# Solar Panel Control System

## Project Overview

This project is a simplified Solar Panel Control System designed to measure the temperature in a solar panel and control a valve (using a servo motor) and a pump (represented by an LED). The system uses an ATmega2560 microcontroller with a VIA-shield.

## Features

- **Temperature Measurement:** Monitors the temperature using a TMP36 sensor.
- **Valve Control:** Opens or closes the valve based on temperature thresholds.
- **Pump Control:** Starts or stops the pump based on temperature thresholds.
- **User Configuration:** Allows user to set `tlow` and `thigh` temperature thresholds using a matrix keyboard.
- **Display:** Shows the current temperature or setpoints on a 7-segment display.
- **Manual Override:** Users can manually view and set `tlow` and `thigh` values.

## Operation

- The current temperature is displayed on the numeric display.
- Pressing switch 1 shows `tlow` on the display. LED 1 turns on (LEDs 2 and 3 turn off).
- Pressing switch 2 shows `thigh` on the display. LED 2 turns on (LEDs 1 and 3 turn off).
- Pressing switch 3 returns the display to the current temperature. LED 3 turns on (LEDs 1 and 2 turn off).
- When the display shows `tlow` or `thigh`, the user can enter a new value using the matrix keyboard and save it by pressing `#` or cancel by pressing `*`.
- LED 6 indicates the valve status (on if open, off if closed).
- LED 7 indicates the pump status (on if running, off if not).

## Additional Details

- The servo motor will turn clockwise when the `thigh` trigger point is reached and counterclockwise when the `tlow` setpoint is reached.
- A timer returns the display to the current temperature 5 seconds after `tlow` or `thigh` have been shown or changed.

## Hardware Used

- Arduino with VIA-shield
- Matrix keyboard
- Servo motor
- TMP36 Temperature Sensor
- LEDs for indicating system status

## Author

- Himal

