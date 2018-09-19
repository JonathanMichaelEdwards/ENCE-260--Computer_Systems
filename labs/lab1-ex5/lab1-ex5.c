#include "pio.h"
#include "system.h"


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


static void ledmat_pixel_set() 
{
    pio_output_toggle(LEDMAT_ROW1_PIO);
    pio_output_toggle(LEDMAT_COL5_PIO);
}


/** Initialise LED matrix PIO pins.  */
static void ledmat_init(void) {
    uint8_t row;
    uint8_t col;

    for (row = 0; row < 7; row++) {
        pio_config_set(ledmat_rows[row], PIO_OUTPUT_HIGH);
    }

    for (col = 0; col < 5; col++) {
        pio_config_set(ledmat_cols[col], PIO_OUTPUT_HIGH);
    }
}


int main (void)
{
    uint8_t col = 0;
    uint8_t row = 4;

    ledmat_init();

    ledmat_pixel_set(row, col);
    
    while (1) {
        continue;
    }

    return 0; 
}