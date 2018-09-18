#ifndef BUTTON_H
#define BUTTON_H

#define BIT(X) (1 << (X))


/** Return non-zero if button pressed.  */
int buttonPressed(uint8_t pin, char bit);


/** Initialise button1.  */
void buttonInit(uint8_t pin, char bit);
#endif
