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


/** Turn single LED within matrix on or off.
    @param col LED column number
    @param row LED row number
    @param state LED state  */
static void ledmat_pixel_set(int col, int row) 
{
    pio_output_toggle(ledmat_rows[row]);
    pio_output_toggle(ledmat_cols[col]);
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