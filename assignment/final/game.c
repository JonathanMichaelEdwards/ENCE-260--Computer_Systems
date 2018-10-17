// ****************************************************************************
// File:     game.c                                                           *
// Authors:  Jonathan Edwards & Ben Empson                                    *
// Date:     17 Oct 2018                                                      *
// Descr:    Everything is initialized and updated here                       *
// ****************************************************************************


#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "led.h"
#include "../fonts/font3x5_1.h"
#include "animations.h"


void initialize(void)
/*
 *    Initializing core components.    
*/
{
    system_init();
    navswitch_init();
    ir_uart_init();
    pacer_init(LOOP_RATE);

    // Initialize blue Led.
    led_init ();
    led_set(LED1, LOW);

    // Initialize tinygl and start message.
    tinygl_init(LOOP_RATE);
    tinygl_font_set(&font3x5_1);
    tinygl_text_speed_set(MESSAGE_RATE_START);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
    tinygl_text(START_MESSAGE);
}


void update(void)
/*
 *    Updating core components.    
*/
{
    pacer_wait();
    navswitch_update();
    tinygl_update();
}


int main (void)
{
    // Initialize.
    initialize();

    while(1) {
        // Updates animations.
        animationUpdate();
        // Update.
        update();
    }
}