// ****************************************************************************
// File:     animations.h                                                     *
// Authors:  Jonathan Edwards & Ben Empson                                    *
// Date:     17 Oct 2018                                                      *
// Descr:    Header file for animations.c                                      *
// ****************************************************************************


#ifndef ANIMATIONS_H
#define ANIMATIONS_H


// Setting Pin.
#define HIGH 1
#define LOW 0

// Collision stopper.
#define MARGIN 1         

// Setting player on map.
#define BOARD_X 2        // Player x location on the bord
#define BOARD_Y 3        // Player y location on the bord

// Flashing rate, limits and speed for bullets.
#define FLASH_LOWER 0
#define ENEMY_FLASH_UPPER 85
#define ENEMY_FLASH_RATE 110 
#define BULLET_SPEED 30

// Rate in Hz.
#define LOOP_RATE 300  

// Health and damage.
#define HEALTH_1 0
#define HEALTH_2 2
#define HEALTH_3 3
#define DAMAGE_FLASH_UPPER 25
#define DAMAGE_FLASH_RATE 50

// Pre and Post Messages.
#define MESSAGE_RATE_START 35
#define MESSAGE_RATE_END 25
#define START_MESSAGE "   CALL OF DUTY - PRESS START"
#define WINNING_MESSAGE " YOU WIN"
#define LOST_MESSAGE " GAME OVER"

// Timer delay.
#define LED_TIMER 350 

// Sending directions.
#define NORTH -5
#define SOUTH -2
#define EAST -3
#define WEST -4

// Sending game flags.
#define START -6
#define END -7

// Sending bullet flags.
#define MELLE -9
#define HIT -8

// Set players x & y coordinates on the map.
#define PIXEL_SET_X ((mapPos.x - playerPos.x) + BOARD_X)
#define PIXEL_SET_Y ((mapPos.y - playerPos.y) + BOARD_Y)

// Keep track of enemy and player positions.
#define ENEMY_POSITION *(map + enemyPos.y * COLS + enemyPos.x)
#define PLAYER_POSITION *(map + (playerPos.y) * COLS + (playerPos.x))

// Enemy shooting on the map.
#define ENEMY_SHOOT_VERTICAL(MARGIN) *(map + (enemyPos.y + MARGIN) * COLS + enemyPos.x)
#define ENEMY_SHOOT_HORIZONTAL(MARGIN) *(map + enemyPos.y * COLS + (enemyPos.x+MARGIN))

// Player shooting on the map.
#define SHOOT_VERTICAL(MARGIN) *(map + (mapPos.y + MARGIN) * COLS + mapPos.x)
#define SHOOT_HORIZONTAL(MARGIN) *(map + mapPos.y  * COLS + (mapPos.x + MARGIN))

// Detect collisions with player & walls.
#define VERTICAL_COLLISION(MARGIN) *(map + (playerPos.y + MARGIN) * COLS + (playerPos.x))
#define HORIZONTAL_COLLISION(MARGIN) *(map + (playerPos.y) * COLS + (playerPos.x + MARGIN))

// Updating animations.
void animationUpdate(void);  


#endif // ANIMATIONS_H