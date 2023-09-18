// Description: This file implements the initialization of an external
// switch.
//----------------------------------------------------------------------//

#include "switch.h"
#include <avr/io.h>

/*
 * Initializes pull-up resistor on PB3 and sets it into input mode
 */
void initSwitchPB3(){
    // PB3 is pin 50 on the breadboard
    DDRB &= ~(1 << DDB3); // set DDB3 as the input
    PORTB |= (1 << PORTB3); // pull-up resistor initialized
    
    // Enable the Pin-Change Interrupt for Port B
    PCICR |= (1 << PCIE0);

    // Enable PB3 for Pin-Change Interrupt
    PCMSK0 |= (1 << PCINT3);
}
