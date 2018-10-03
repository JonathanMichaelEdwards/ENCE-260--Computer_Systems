/*
    Written by: Jonathan Edwards
*/

#include <avr/io.h>


int main (void)
{
    DDRC = (1 << 2);
    
    while(1) {
        PORTC = (1 << 2);
    }
}
 