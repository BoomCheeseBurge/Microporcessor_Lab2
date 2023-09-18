// Author:         Brandon John Lim Yung Chen
// Net ID:         
// Date:          
// Assignment:     Lab 2
//
// Description: This file contains a programmatic overall description of the
// program. It should never contain assignments to special function registers
// for the exception key one-line code such as checking the state of the pin.
//
//----------------------------------------------------------------------//
#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h> // Library containing all possible interrupts across all AVR micro-controllera
#include "led.h"
#include "switch.h"
#include "timer.h"
// Define the delay constants for slow and fast delay
#define SLOW_DELAY 200
#define FAST_DELAY 100

/*
 * Define a set of states that can be used in the state machine using an enum.
 */
typedef enum stateType_enum {
wait_press, debounce_press,
wait_release, debounce_release
} stateType;

// Initialize state
// volatile is used since the state value changes outside the control of the code
volatile stateType state = wait_press;

// Define a global variable to toggle the led speed (1 is slow and 2 is fast)
int delay_speed = 1;  // initially set to slow

int main(){

	// Enable global interrupt for the ISR to run
	sei();

	// Initialize the LED pins as outputs
	initLED();

	// Initialize the switch
	initSwitchPB3();

    // Enable individual ISR
	initTimer0();

	// Initialize counter variable
	unsigned char count = 0;
  
	/*
	* Implement a state machine in the while loop which achieves the assignment
	* requirements.
	*/
	while (1) {

		// if switch speed is set for fast from ISR
		if (delay_speed == 2) {

			delayMs(FAST_DELAY);
			turnOnLEDWithChar(count);

		} else {  // else switch speed is set for slow from ISR

			delayMs(SLOW_DELAY);
			turnOnLEDWithChar(count);
		}

		// Switch case for handling debounce
		switch(state) {
			case wait_press:
				break;
			case debounce_press:
				delayMs(1);
				state = wait_release;
				break;
			case wait_release:
				break;
			case debounce_release:
				delayMs(1);
				state = wait_press;
				break;
		}

		count = (count + 1) % 16; // Increment count and wrap around at 15
	}
  	return 0;
}

/* Implement an Pin Change Interrupt which handles the switch being
* pressed and released. When the switch is pressed and released, the LEDs
* change at twice the original rate. If the LEDs are already changing at twice
* the original rate, it goes back to the original rate.
*/
ISR(PCINT0_vect){
  
	if(state == wait_press){
		state = debounce_press;
	}
	else if(state == wait_release){
		state = debounce_release;
		if (delay_speed == 2) {
			delay_speed = 1; // Change to 100ms delay
		} else {
			delay_speed = 2; // Change back to 200ms delay
		}
	}
}