#include <avr/io.h>
#include "system.h"
#include "led.h"
#include "pio.h"

#define SEC_TO_MILL 1000
#define TIMER_PRESCALE 1024  // 2^10
#define TIME_DELAY(DELAY) (DELAY * (F_CPU / TIMER_PRESCALE) / SEC_TO_MILL)

int main (void)
{
    system_init();
    led_init();
    
    /* TODO: Initialise timer/counter1.  */
    TCCR1A = 0x00; 
    TCCR1B = 0x05; 
    TCCR1C = 0x00;
    
    while (1)
    {
        /* Turn LED on.  */
        led_set(LED1, 1);
        /* TODO: wait for 500 milliseconds.  */
        TCNT1 = 0;
        while (TCNT1 < TIME_DELAY(500)) continue; 

        /* Turn LED off.  */
        led_set(LED1, 0);
        /* TODO: wait for 500 milliseconds.  */
        TCNT1 = 0;
        while (TCNT1 < TIME_DELAY(500)) continue; 
    }
    
}
