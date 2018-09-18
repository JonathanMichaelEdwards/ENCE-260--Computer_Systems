#include <avr/io.h>
#include "button.h"


/** Return non-zero if button pressed.  */
int buttonPressed(uint8_t pin, char bit) {
    return (pin & BIT(bit));
}


/** Initialise button1.  */
void buttonInit(uint8_t pin, char bit) {
    pin &= ~BIT(bit);
}