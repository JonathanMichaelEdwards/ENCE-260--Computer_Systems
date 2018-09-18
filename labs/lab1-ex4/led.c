#include <avr/io.h>
#include "led.h"


/** Switch led on or off */
void ledSwitch(volatile uint8_t *port, char bit) {   // volatile since port could be changed during runtime.
    *port ^= BIT(bit);
}


/** Initialise LED1.  */
void ledInit(volatile uint8_t *ddr, char bit) {   
    *ddr |= BIT(bit);
}
