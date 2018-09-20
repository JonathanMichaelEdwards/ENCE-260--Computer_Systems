#include <avr/io.h>
#include "button.h"
#include "led.h"
#include "pio.h"

#define DEBOUNCE_MAX 100
#define LED_BIT 2
#define BUTTON_BIT 7
PIO_OUTPUT_LOW


#define LED_INIT ledInit(&DDRC, LED_BIT)
#define BUTTON_INIT buttonInit(DDRD, BUTTON_BIT);


int main (void)
{
    char pressed = 0;
    char btnPressed = 0;
    char btnReleased = 0;

    LED_INIT;
    BUTTON_INIT;
    PORTC = BIT(LED_BIT);

    while (1) {
        if (buttonPressed(PIND, BUTTON_BIT)) {
            if (btnPressed > DEBOUNCE_MAX) {
                if (pressed == 0) {
                    ledSwitch(&PORTC, LED_BIT);
                    pressed = 1;
                    btnPressed = 0;
                }
            }
        } else {
            if (btnReleased > DEBOUNCE_MAX) {
                pressed = 0;
                btnReleased = 0;
            }
        }
        btnPressed++;
        btnReleased ++;
    }

    return 0; 
}
