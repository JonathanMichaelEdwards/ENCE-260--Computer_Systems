#include <avr/io.h>
#include "timer.h"

#define SEC_TO_MILL 1000
#define TIMER_PRESCALE 1024  // 2^10
#define DELAY_TICKS(DELAY) (DELAY * (F_CPU / TIMER_PRESCALE) / SEC_TO_MILL)


/* Initialise timer.  */
void timer_init(void)
{
    TCCR1A = 0x00; 
    TCCR1B = 0x05; 
    TCCR1C = 0x00;
}


/* Wait for the specified length of time.  */
void timer_delay_ms(uint16_t milliseconds)
{   
    TCNT1 = 0;
    while (TCNT1 < DELAY_TICKS(milliseconds)) continue; 
}
