#ifndef BUTTON_H
#define BUTTON_H

#define BIT(X) (1 << (X))


/** Return non-zero if button pressed.  */
int button_pressed_p (void);


/** Initialise button1.  */
void button_init (void);
#endif
