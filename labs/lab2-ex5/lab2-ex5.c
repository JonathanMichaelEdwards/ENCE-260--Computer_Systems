#include "system.h"
#include "pio.h"
#include "pacer.h"


/** Define PIO pins driving LED matrix rows.  */
static const pio_t rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO, 
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};


/** Define PIO pins driving LED matrix columns.  */
static const pio_t cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};


static const uint8_t bitmap[] =
{
    0x30, 0x46, 0x40, 0x46, 0x30
};


static void ledmat_init(void) {
    for (uint8_t row = 0; row < 7; row++) {
        pio_config_set(rows[row], PIO_OUTPUT_HIGH);
    }

    for (uint8_t col = 0; col < 5; col++) {
        pio_config_set(cols[col], PIO_OUTPUT_HIGH);
    }
}


static void display_column(uint8_t row_pattern, uint8_t current_column)
{
    if ((row_pattern >> current_column)) {
        pio_output_low(rows[current_column]);
    } else {
        pio_output_high(rows[current_column]);
    }
}


int main (void)
{
    uint8_t current_column = 0;
    
    system_init();
    pacer_init(500);
    
    /* TODO: Initialise LED matrix pins.  */
    ledmat_init();

    while (1) {
        
        pacer_wait();
        
        display_column(bitmap[current_column], current_column);
        
        current_column++;  

        // if (current_column > (LEDMAT_COLS_NUM - 1)) {
        //     current_column = 0;
        // }        
    }
}
