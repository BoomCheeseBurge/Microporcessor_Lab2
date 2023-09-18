// Description: This file implements functions that utilize the timers
//----------------------------------------------------------------------//

#include "timer.h"

/* Initialize timer 0, you should not turn the timer on here.
* You will need to use CTC mode */
void initTimer0(){

    // Enable CTC mode timer 0 module
    TCCR0A &= ~ (1 << WGM00);
    TCCR0A |= (1<< WGM01);
    TCCR0B &= ~(1 << WGM02);

    // Set the number of ticks per cycle
    OCR0A = 249;
}

/* This delays the program an amount specified by unsigned int delay.
* Use timer 0. Keep in mind that you need to choose your prescalar wisely
* such that your timer is precise to 1 millisecond and can be used for
* 100-200 milliseconds
*/
void delayMs(unsigned int delay){
    unsigned int count = 0;

    // Start the clock timer by setting the prescaler value
    // Prescaler value is set to 64
    TCCR0B |= (1 << CS00) | (1 << CS01);
    TCCR0B &= ~ (1 << CS02);

    while(count < delay){

        // Put thE output compare match flag down  (OCF0A) by writing a logic 1
        TIFR0 |= (1 << OCF0A);
        TCNT0 = 0;  // CLEAR the count register

        // poll the OCF0A flag until it is set to logic 1
        // Basically wait until the clock finishes one cycle of 1ms timer
        while (! (TIFR0 & (1 << OCF0A)));  // The flag is down while the clock finishes its ticks

        // After the flag is raised from the condition above, increment the counter
        count++;
    }

    // After counter has stopped, turn off the Timer0 clock
    // All clock selectors are set 0 for no clock source
    TCCR0B &= ~((1 << CS00) | (1 << CS01) | (1 << CS02));
}
