#include <avr/io.h>
#include "system.h"

#define BIT(X) (1 << (X))



static void led_init (void)
{
    /* Initialise port to drive LED 1.  */
    DDRC |= BIT(2);
}


static void led_on (void)
{
    /* Set port to turn LED 1 on.  */
    PORTC |= BIT(2);
}


static void led_off (void) {
    /* Set port to turn LED 1 off.  */
    PORTC = BIT(0);
}


static void button_init (void) {
    /* Initialise port to read button 1.  */
    DDRD &= ~BIT(7);
}


static int button_pressed_p (void) {
    /* Return non-zero if button pressed_p.  */
    return (PIND &= BIT(7));
}


int main (void) {
    system_init();

    led_init();
    button_init();

    while (1) {
        if (button_pressed_p()) {
            led_on ();
        } else {
            led_off ();
        }
    }
}
