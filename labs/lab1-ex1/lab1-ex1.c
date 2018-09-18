/* @file   lab1-exc1.c
    @author Jonathan Edwards
    @date   15 Sept 2018
*/

#include <avr/io.h>
#include "system.h"

#define BIT(X) (1 << (X))


int main (void)
{
    /* Initialise port to drive LED 1.  */
    DDRC |= BIT(2);
    DDRD &= ~BIT(7);

    
    while (1)
    {
        /* Turn LED 1 on when s3 pressed */
        if (PIND &= BIT(7)) {
            PORTC |= BIT(2);
        } else {
            PORTC = BIT(0);
        }
    }
}
