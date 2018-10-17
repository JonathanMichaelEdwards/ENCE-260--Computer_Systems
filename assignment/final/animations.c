// ****************************************************************************
// File:     animations.c                                                     *
// Authors:  Jonathan Edwards & Ben Empson                                    *
// Date:     17 Oct 2018                                                      *
// Descr:    Creates the world, movement, collisions and communications       *
// ****************************************************************************


#include <stdbool.h>
#include "animations.h"
#include "maps.h"
#include "tinygl.h"
#include "ir_uart.h"
#include "navswitch.h"
#include "button.h"
#include "led.h"


// Vector x & y coordinates
tinygl_point_t playerStartPos = {0, 0};
tinygl_point_t enemyStartPos = {0, 0};
tinygl_point_t playerPos = {0, 0};
tinygl_point_t enemyPos = {0, 0};
tinygl_point_t mapPos = {0, 0};

// Running time states
bool enemyPlayerFound = false;
bool playerFound = false;

// Set to true to start the game
bool startGame = false;

// Character choice 
uint8_t player = 0;
uint8_t enemyPlayer = 0;

// Declare moving direction
uint8_t direction = 0;

// Timers
int8_t flashPlayer = 0;
int8_t flashEnemy = 0;
int8_t bulletTimer = 0;
int8_t bulletTimerEnemy = 0;
int16_t ledTimer = 0;

// Player hit points
uint8_t hp = 0;

// Flashing rate & limits for player
uint8_t flash_upper = 80;
uint8_t flash_rate = 160; 


void reset(void) 
/*
 *    Reseting Global Varibles, To Start Game Again.     
*/
{
    // Reset flashing rate & limits for player.
    flash_upper = 80;
    flash_rate = 160; 

    // Reset player's starting points.
    playerPos.x = playerStartPos.x;
    playerPos.y = playerStartPos.y;
    enemyPos.x = enemyStartPos.x;
    enemyPos.y = enemyStartPos.y;
    
    // Reset health and start directon.
    hp = 3;
    direction = 0;

    // Reset timers.
    flashPlayer = 0;
    flashEnemy = 0;
    bulletTimer = 0;
    bulletTimerEnemy = 0;
    ledTimer = 0;

    // Reset running time states.
    enemyPlayerFound = false;
    playerFound = false;
}


void gameOver(uint8_t *map) 
/*
 *    Showing Post Game Message, Push Navswitch To Restart. 
*/
{
    if (hp == HEALTH_1) {
        // Player Lost Game.
        tinygl_text_speed_set(MESSAGE_RATE_END);
        tinygl_text(LOST_MESSAGE);

        // Resets starting positions.
        PLAYER_POSITION = '.';
        ENEMY_POSITION = '.';
        PLAYER_POSITION = '1';
        ENEMY_POSITION = '2';

        // Resets varibles.
        reset();
    } else {
        // Player Won Game.
        tinygl_text_speed_set(MESSAGE_RATE_END);
        tinygl_text(WINNING_MESSAGE);

        // Resets starting positions.
        PLAYER_POSITION = '.';
        ENEMY_POSITION = '.';
        PLAYER_POSITION = '2';
        ENEMY_POSITION = '1';

        // Resets varibles.
        reset();
    }
}


void damage(uint8_t *map) 
/*
 *    Checks How Much Health A Player Has When Recieving Damage.     
*/
{
    ir_uart_putc(HIT);
    if (hp == HEALTH_3) {
        hp--;
    } else if (hp == HEALTH_2) {
        flash_upper = DAMAGE_FLASH_UPPER;
        flash_rate = DAMAGE_FLASH_RATE;
        hp--;
    } else {
        hp--;
        ir_uart_putc(END);
        gameOver(map);
    }
}


void enemyFlash(uint8_t *map)
/*
 *    Takes A Input *map To Check Where The Enemy Should Flash On The Map. 
 *    Enemy Flashes At Roughly 250 ms. const uint8_t *string = "Hello World";
*/
{ 
    if (MAP_SET == enemyPlayer) {
        if (FLASH_LOWER < flashEnemy && flashEnemy < ENEMY_FLASH_UPPER) {
            display_pixel_set(PIXEL_SET_X, PIXEL_SET_Y, HIGH);
        }
        if (ENEMY_FLASH_UPPER <= flashEnemy) {
            display_pixel_set(PIXEL_SET_X, PIXEL_SET_Y, LOW);
            flashEnemy -= ENEMY_FLASH_RATE;
        }
    }
}


void playerFlash(uint8_t *map) 
/*
 *    Takes A Input *map To Check Where The Player Should Flash On The Map. 
 *    Plyer Flashes At Different Speeds Depending On Current HP.      
*/
{ 
    if (MAP_SET == player) {
        if ((FLASH_LOWER < flashPlayer && flashPlayer < flash_upper) || (hp == HEALTH_3)) {
            display_pixel_set(PIXEL_SET_X, PIXEL_SET_Y, HIGH);
        }
        if (flash_upper <= flashPlayer) {
            display_pixel_set(PIXEL_SET_X, PIXEL_SET_Y, LOW);
            flashPlayer -= flash_rate;
        }
    }
}


void sendIFR(void)
/*
 *    Takes An Input playerPos And Transmites The 1 Byte Of Information To
 *    The Opponent Roughly Every 150 microseconds.
*/
{
    uint8_t coords = 0;
    // Concatinating players position.
    coords |= playerPos.y;
    coords |= (playerPos.x << 4);
    ir_uart_putc(coords);
}


void recieveIFR(uint8_t *map)
/*
 *    Locates The Position Of Your Opponent. Resets The Timer When A Bullet 
 *    Is Shot. Since The ir_uart_getc Function Returns A signed Value, Meaning 
 *    We Can Recieve values of -128 to 127. Therfore We Can Recieve Negative 
 *    Numbers For Shooting And Positive Numbers For Our Player's Positions.            
*/
{   
    if (ir_uart_read_ready_p()) {
        int8_t output = ir_uart_getc();
        if (0 <= output && output < 127) {
            // Prevents wall breaking for enemy player moving.
            if (ENEMY_POSITION == enemyPlayer) {
                ENEMY_POSITION = '.';
            }
            // Unpack enemy's positions being recieved.
            enemyPos.y = output & 0x0F;
            enemyPos.x = (output & 0xF0) >> 4;
            if (ENEMY_POSITION == '.') {
                ENEMY_POSITION = enemyPlayer;
            }
        } else if (output == NORTH) {
            // Prevents wall breaking for bullet shooting.
            if (ENEMY_SHOOT_VERTICAL(-MARGIN) == '.') {
                ENEMY_SHOOT_VERTICAL(-MARGIN) = 'N';
                bulletTimerEnemy = 0;
            }
        } else if (output == SOUTH) {
            if (ENEMY_SHOOT_VERTICAL(MARGIN) == '.') {
                ENEMY_SHOOT_VERTICAL(MARGIN) = 'S';
                bulletTimerEnemy = 0;
            }
        } else if (output == EAST) {
            if (ENEMY_SHOOT_HORIZONTAL(MARGIN) == '.') {
                ENEMY_SHOOT_HORIZONTAL(MARGIN) = 'E';
                bulletTimerEnemy = 0;
            }
        } else if (output == WEST) {
            if (ENEMY_SHOOT_HORIZONTAL(-MARGIN) == '.') {
                ENEMY_SHOOT_HORIZONTAL(-MARGIN) = 'W';
                bulletTimerEnemy = 0;
            }
        } else if (output == END) {
            gameOver(map);  // Sends players a post message.
        } else if (output == MELLE) {
            damage(map);  // Reduces health.
        } else if (output == HIT) {
            led_set(LED1, HIGH);  // Turn Led on if opponent was hit.
            ledTimer = LED_TIMER;
        } 
    }
}


void shootNorth(uint8_t *map)
/*
 *    Checks That The Player Is Shooting North, And His Map Location.         
*/
{
    if (bulletTimer < BULLET_SPEED) {
        display_pixel_set(PIXEL_SET_X, PIXEL_SET_Y, HIGH);
    } else {
        if (SHOOT_VERTICAL(-MARGIN) != '.') {
            MAP_SET = '.';
        } else {
            MAP_SET = '.';
            SHOOT_VERTICAL(-MARGIN) = 'n';
        } 
    }
}


void shootSouth(uint8_t *map)
/*
 *    Checks That The Player Is Shooting South At His Map Location.          
*/
{
    if (bulletTimer < BULLET_SPEED) {
        display_pixel_set(PIXEL_SET_X, PIXEL_SET_Y, HIGH);
    } else {
        // South bullets disapear without the '!' checker due to map setup.
        if (SHOOT_VERTICAL(-MARGIN) == '!') {
            SHOOT_VERTICAL(-MARGIN) = '.';
        } else if (SHOOT_VERTICAL(MARGIN) != '.') {
            MAP_SET = '.';  // Clear pixel off screen.
        } else {
            MAP_SET = '!';
            SHOOT_VERTICAL(MARGIN) = 's';
        }
    } 
}


void shootEast(uint8_t *map)
/*
 *    Checks That The Player Is Shooting East At His Map Location.         
*/
{
    if (bulletTimer < BULLET_SPEED) {
        display_pixel_set(PIXEL_SET_X, PIXEL_SET_Y, HIGH);
    } else {
        // East bullets disapear without the '!' checker due to map setup.
        if (SHOOT_HORIZONTAL(-MARGIN) == '!') {
            SHOOT_HORIZONTAL(-MARGIN) = '.';
        } else if (SHOOT_HORIZONTAL(MARGIN) != '.') {
            MAP_SET = '.';  // Clear pixel off screen.
        } else {
            MAP_SET = '!';
            SHOOT_HORIZONTAL(MARGIN) = 'e';
        }
    }
}


void shootWest(uint8_t *map)
/*
 *    Checks That The Player Is Shooting West At His Map Location.        
*/
{
    if (bulletTimer < BULLET_SPEED) {
        display_pixel_set(PIXEL_SET_X, PIXEL_SET_Y, HIGH);
    } else {
        if (SHOOT_HORIZONTAL(-MARGIN) != '.') {
            MAP_SET = '.';  // Clear pixel off screen.
        } else {
            MAP_SET = '.';
            SHOOT_HORIZONTAL(-MARGIN) = 'w';
        }
    }
}


void shootNorthEnemy(uint8_t *map)
/*
 *    Checks That The Enemy Is Shooting North At His Map Location.         
*/
{
    if (bulletTimerEnemy < BULLET_SPEED) {
        display_pixel_set(PIXEL_SET_X, PIXEL_SET_Y, HIGH);
    } else {
        if (SHOOT_VERTICAL(-MARGIN) != '.') {
            MAP_SET = '.';  // Clear pixel off screen.
            if (SHOOT_VERTICAL(-MARGIN) == player) {
                damage(map);  // Reduce player health.
            }
        } else {
            MAP_SET = '.';
            SHOOT_VERTICAL(-MARGIN) = 'N';
        }
    }
}


void shootSouthEnemy(uint8_t *map)
/*
 *    Checks That The Enemy Is Shooting South At His Map Location.         
*/
{
    if (bulletTimerEnemy < BULLET_SPEED) {
        display_pixel_set(PIXEL_SET_X, PIXEL_SET_Y, HIGH);
    } else {
        // South bullets disapear without the '!' checker due to map setup.
        if (SHOOT_VERTICAL(-MARGIN) == '!') {
            SHOOT_VERTICAL(-MARGIN) = '.';
        } else if (SHOOT_VERTICAL(MARGIN) != '.') {
            MAP_SET = '.';  // Clear pixel off screen.
            if (SHOOT_VERTICAL(MARGIN) == player)  {
                damage(map);  // Reduce player health.
            }
        } else {
            MAP_SET = '!';
            SHOOT_VERTICAL(MARGIN) = 'S';
        }
    }
}


void shootEastEnemy(uint8_t *map)
/*
 *    Checks That The Enemy Is Shooting East At His Map Location.        
*/
{
    if (bulletTimerEnemy < BULLET_SPEED) {
        display_pixel_set(PIXEL_SET_X, PIXEL_SET_Y, HIGH);
    } else {
        // East bullets disapear without the '!' checker due to map setup.
        if (SHOOT_HORIZONTAL(-MARGIN) == '!') {
            SHOOT_HORIZONTAL(-MARGIN) = '.';
        } else if (SHOOT_HORIZONTAL(MARGIN) != '.') {
            MAP_SET = '.';  // Clear pixel off screen.
            if (SHOOT_HORIZONTAL(MARGIN) == player) {
                damage(map);  // Reduce player health.
            }
        } else {
            MAP_SET = '!';
            SHOOT_HORIZONTAL(MARGIN) = 'E';
        } 
    }
}


void shootWestEnemy(uint8_t *map)
/*
 *    Checks That The Enemy Is Shooting West At His Map Location.         
*/
{
    if (bulletTimerEnemy < BULLET_SPEED) {
        display_pixel_set(PIXEL_SET_X, PIXEL_SET_Y, HIGH);
    } else {
        if (SHOOT_HORIZONTAL(-MARGIN) != '.') {
            MAP_SET = '.';  // Clear pixel off screen.
            if (SHOOT_HORIZONTAL(-MARGIN) == player) {
                damage(map);
            }
        } else {
            MAP_SET = '.';
            SHOOT_HORIZONTAL(-MARGIN) = 'W';
        }
    }
}


void playerShoot(uint8_t *map) 
/*
 *    Checks If A Bullet Is That The Current Map Coord, And Moves It.           
*/
{
    if (MAP_SET == 'n') {
        shootNorth(map);
    } else if (MAP_SET == 's') {
        shootSouth(map);
    } else if (MAP_SET == 'e') {
        shootEast(map);
    } else if (MAP_SET == 'w') {
        shootWest(map);
    } else if (MAP_SET == 'N') {
        shootNorthEnemy(map);
    } else if (MAP_SET == 'S') {
        shootSouthEnemy(map);
    } else if (MAP_SET == 'E') {
        shootEastEnemy(map);
    } else if (MAP_SET == 'W') {
        shootWestEnemy(map);
    }
}


void bulletShot(uint8_t *map)
/*
 *    Checking If And Where The Bullet Should Be Shot On The Map. 
*/
{
    if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
        // Finding whether the player can shoot a bullet.
        if (direction == 'N' && VERTICAL_COLLISION(-MARGIN) != 'X' && 
            VERTICAL_COLLISION(-MARGIN * 2) != 'n') {
            if (VERTICAL_COLLISION(-MARGIN) == enemyPlayer) {
                ir_uart_putc(MELLE);  // Close range hit.
                led_set(LED1, HIGH);  // Player Hit, turns Led on.
                ledTimer = LED_TIMER;
            } else {
                VERTICAL_COLLISION(-MARGIN) = 'n';
                ir_uart_putc(NORTH);
            }
        } else if (direction == 'S' && VERTICAL_COLLISION(MARGIN) != 'X' && 
            VERTICAL_COLLISION(MARGIN * 2) != 's') {
            if (VERTICAL_COLLISION(MARGIN) == enemyPlayer) {
                ir_uart_putc(MELLE);  // Close range hit.
                led_set(LED1, HIGH);  // Player Hit, turns Led on.
                ledTimer = LED_TIMER;
            } else {
                VERTICAL_COLLISION(MARGIN) = 's';
                ir_uart_putc(SOUTH);
            }
        } else if (direction == 'E' && HORIZONTAL_COLLISION(MARGIN) != 'X' && 
            HORIZONTAL_COLLISION(MARGIN * 2) != 'e') {
            if (HORIZONTAL_COLLISION(MARGIN) == enemyPlayer) {
                ir_uart_putc(MELLE);  // Close range hit.
                led_set(LED1, HIGH);  // Player Hit, turns Led on.
                ledTimer = LED_TIMER;
            } else {
                HORIZONTAL_COLLISION(MARGIN) = 'e';
                ir_uart_putc(EAST);
            }
        } else if (direction == 'W' && HORIZONTAL_COLLISION(-MARGIN) != 'X' && 
            HORIZONTAL_COLLISION(-MARGIN * 2) != 'w') {
            if (HORIZONTAL_COLLISION(-MARGIN) == enemyPlayer) {
                ir_uart_putc(MELLE);  // Close range hit.
                led_set(LED1, HIGH);  // Player Hit, turns Led on.
                ledTimer = LED_TIMER;
            } else {
                HORIZONTAL_COLLISION(-MARGIN) = 'w';
                ir_uart_putc(WEST);
            }
        }
        // Send bord locations to opponent.
        sendIFR();  
        bulletTimer = 0;
    }
}


void timer(void)
/*
 *    Wait Roughly 300 ms To Shoot Bullet Again.  
*/
{
    if (bulletTimer >= BULLET_SPEED) {
        bulletTimer = 0;
    }
    if (bulletTimerEnemy >= BULLET_SPEED) {
        bulletTimerEnemy = 0;
    }
    if (ledTimer > 1) {
        ledTimer--;
    }
    if (ledTimer <= 1) {
        led_set(LED1, LOW);
        ledTimer--;
    }
    bulletTimer++;
    bulletTimerEnemy++;
}


uint8_t *worldSetup(void)
/*
 *    Builds The World By Finding The locations Of Both Players And Walls. 
 *    Returns The *map Generated By Them.
*/
{
    // Map generated when game is started/reseted.
    uint8_t *map = getMap();
    
    for (mapPos.y = 0; mapPos.y < ROWS; mapPos.y++) {
        for (mapPos.x = 0; mapPos.x < COLS; mapPos.x++) {
            // Find all locations of both playes and walls on the map.
            if (playerFound && enemyPlayerFound) {
                if (MAP_SET == 'X') {
                    // Draws map outline.
                    display_pixel_set(PIXEL_SET_X, PIXEL_SET_Y, HIGH);
                } else {
                    playerFlash(map);
                    enemyFlash(map);
                    playerShoot(map);
                }
            } else if (MAP_SET == player) {
                // Set players positions.
                playerPos.x = mapPos.x;
                playerPos.y = mapPos.y;
                // Stores players start positions.
                playerStartPos.x = mapPos.x;
                playerStartPos.y = mapPos.y;
                playerFound = true;
            } else if (MAP_SET == enemyPlayer) {
                // Set enemy players positions on the map.
                enemyPos.x = mapPos.x; 
                enemyPos.y = mapPos.y;
                // Stores enemies start positions.
                enemyStartPos.x = mapPos.x;
                enemyStartPos.y = mapPos.y;
                enemyPlayerFound = true;
            }
        } 
    }
    
    return map;
}


void moveNorth(uint8_t *map)
/*
 *     Takes An Input *map To Check Whether A Collision Hasn't Occured With Both Player
 *     And The Wall While Moving North.            
*/
{
    if (navswitch_push_event_p(NAVSWITCH_NORTH)) {
        direction = 'N';
        // When no collision occurs, the player can move.
        if (VERTICAL_COLLISION(-MARGIN) == '.' || VERTICAL_COLLISION(-MARGIN) == 'N' ||
            VERTICAL_COLLISION(-MARGIN) == 'S' || VERTICAL_COLLISION(-MARGIN) == 'E' ||
            VERTICAL_COLLISION(-MARGIN) == 'W') {
            if (VERTICAL_COLLISION(-MARGIN) != '.') {
                damage(map);
            }
            PLAYER_POSITION = '.';
            playerPos.y--;  // Update players y-direction
            PLAYER_POSITION = player;
            sendIFR();  // Send players position to opponent.
        }
    }
}
                              

void moveSouth(uint8_t *map)
/*
 *     Takes An Input *map To Check Whether A Collision Hasn't Occured With Both Player
 *     And The Wall While Moving South.            
*/
{
    if (navswitch_push_event_p(NAVSWITCH_SOUTH)) {
        direction = 'S';
        // When no collision occurs, the player can move.
        if (VERTICAL_COLLISION(MARGIN) == '.' || VERTICAL_COLLISION(MARGIN) == 'N' ||
            VERTICAL_COLLISION(MARGIN) == 'S' || VERTICAL_COLLISION(MARGIN) == 'E' ||
            VERTICAL_COLLISION(MARGIN) == 'W') {
            if (VERTICAL_COLLISION(MARGIN) != '.') {
                damage(map);
            }
            PLAYER_POSITION = '.';
            playerPos.y++;  // Update players y-direction.
            PLAYER_POSITION = player;
            sendIFR();  // Send players position to opponent.
        }
    }
}


void moveEast(uint8_t *map)
/*
 *     Takes An Input *map To Check Whether A Collision Hasn't Occured With Both Player
 *     And The Wall While Moving East.            
*/
{
    if (navswitch_push_event_p(NAVSWITCH_EAST)) {
        direction = 'E';
        // When no collision occurs, the player can move.
        if (HORIZONTAL_COLLISION(MARGIN) == '.' || HORIZONTAL_COLLISION(MARGIN) == 'N' ||
            HORIZONTAL_COLLISION(MARGIN) == 'S' || HORIZONTAL_COLLISION(MARGIN) == 'E' ||
            HORIZONTAL_COLLISION(MARGIN) == 'W') {
            if (HORIZONTAL_COLLISION(MARGIN) != '.') {
                damage(map);
            }
            PLAYER_POSITION = '.';
            playerPos.x++;  // Update players y-direction.
            PLAYER_POSITION = player;
            sendIFR();  // Send players position to opponent.
        }
    }
}                          


void moveWest(uint8_t *map)
/*
 *     Takes An Input *map To Check Whether A Collision Hasn't Occured With Both Player
 *     And The Wall While Moving West.            
*/
{
    if (navswitch_push_event_p(NAVSWITCH_WEST)) {
        direction = 'W';
        // When no collision occurs, the player can move.
        if (HORIZONTAL_COLLISION(-MARGIN) == '.' || HORIZONTAL_COLLISION(-MARGIN) == 'N' ||
            HORIZONTAL_COLLISION(-MARGIN) == 'S' || HORIZONTAL_COLLISION(-MARGIN) == 'E' ||
            HORIZONTAL_COLLISION(-MARGIN) == 'W') {
            if (HORIZONTAL_COLLISION(-MARGIN) != '.') {
                damage(map); 
            }
            PLAYER_POSITION = '.';
            playerPos.x--;  // Update players y-direction.
            PLAYER_POSITION = player;
            sendIFR();  // Send players position to opponent. 
        }
    }
}


uint8_t *refreshDisplay(void)
/*
 *    Refreshes The Users Bord When The Player Moves And Returns Their Map.     
*/
{
    // Reset display.
    display_clear();

    // Gets the map from the world.
    uint8_t *map = worldSetup();

    // Checks for push events and updates players position.
    moveNorth(map);
    moveSouth(map);
    moveEast(map);
    moveWest(map);

    return map;
}


void chooseCharacter(void)
/*                              
 *    When Navswitch Is Pushed, Starts Or Restarts Game.      
*/
{
    // Game starts when pushed.
    if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
        player = '1';
        enemyPlayer = '2';
        hp = 3;
        tinygl_clear();  // Clearing messages
        startGame = true;  // Allowing game to start.
        ir_uart_putc(START);
        
    }
    if (ir_uart_read_ready_p()) {
        // Checking value recieved from transmitter.
        if (ir_uart_getc() == START) {
            player = '2';
            enemyPlayer = '1';
            hp = 3;
            tinygl_clear();  // Clearing messages
            startGame = true;  // Allowing game to start.
        }
    }
}


void animationUpdate(void)
/*
 *    Updating Animating Components.         
*/
{ 
    // When player selected, start Game.
    if (startGame) {
        // Update player movement, animations and communications.
        uint8_t *map = refreshDisplay();
        recieveIFR(map);
        bulletShot(map);
        // Update timer and flashers.
        timer(); 
        flashEnemy++;  
        flashPlayer++;
    } else {
        chooseCharacter();
    }
}