#include "system.h"
#include "pio.h"

/* Include the pacer module from the previous lab.
   You must have completed this before starting this lab2-exercise.  */
#include "pacer.h"


/* Define PIO pins driving LED matrix rows and columns.  */
static pio_t ledmat_rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO, LEDMAT_ROW4_PIO,
    LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO, LEDMAT_ROW7_PIO
};
static pio_t ledmat_cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};


/** Initialise LED matrix PIO pins.  */
static void ledmat_init(void) {
    for (uint8_t row = 0; row < 7; row++) {
        pio_config_set(ledmat_rows[row], PIO_OUTPUT_HIGH);
    }

    for (uint8_t col = 0; col < 5; col++) {
        pio_config_set(ledmat_cols[col], PIO_OUTPUT_HIGH);
    }
}


int main (void)
{
    system_init ();
    
    /* TODO: Initialise the pins of the LED matrix.  */
    ledmat_init();

    /* Set up pacer with a frequency of 90 Hz.  */
    pacer_init(90);

    while (1)
    {   
        
        /* State 1 Set high and low outputs */
        pio_output_high(LEDMAT_ROW1_PIO);
        pio_output_high(LEDMAT_COL5_PIO);
        pio_output_low(LEDMAT_ROW1_PIO); 
        pio_output_low(LEDMAT_ROW7_PIO);
        pio_output_low(LEDMAT_COL1_PIO); 
       
        // Wait for 90 Hz
        pacer_wait();

        /* State 2, Set high and low outputs */
        pio_output_high(LEDMAT_ROW7_PIO);
        pio_output_low(LEDMAT_ROW1_PIO); 
        pio_output_low(LEDMAT_COL5_PIO); 

        // Wait for 90 Hz
        pacer_wait();
    }
}
