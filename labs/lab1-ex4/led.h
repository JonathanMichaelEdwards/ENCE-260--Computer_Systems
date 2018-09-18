#ifndef LED_H
#define LED_H

#define BIT(X) (1 << (X))

/** Switch led on or off */
void ledSwitch(volatile uint8_t *port, char bit);


/** Initialise LED1.  */
void ledInit(volatile uint8_t *ddr, char bit);
#endif
