#ifndef __MOVE_XPM_H
#define __MOVE_XPM_H

#include "xpm/xpm_config.h"

/** @defgroup game game
 * @{
 *
 * Functions to control all the environment of the game
 */

/**
 * @brief Enumerated type for specifying the current state of the game
*/
typedef enum{
    MAINMENU, /*!<Game state is currently in the main menu*/
    GAME_BACKGROUNG, /*!<Game state is currently in the game background drawing the xpm's in the screen*/
    GAME, /*!<Game state is currently in the main menu*/
    WINNER, /*!<Game state is currently in the winner screen*/
    EXIT /*!<Game state is currently exiting*/
    }Game_State;

/**
 * @brief Enumerated type for specifying the current time of the day, to be used in two modes of the game (day and night)
*/
typedef enum {
    DAY, /*!<Day time*/
    NIGHT /*!<Night time*/
    }Time_Day;

/**
 * @brief Enumerated type for specifying the current image of the sprite
*/
typedef enum {
    IMAGE1, /*!<Current image of the sprite is 1*/
    IMAGE2, /*!<Current image of the sprite is 2*/
    IMAGE3, /*!<Current image of the sprite is 3*/
    IMAGE4, /*!<Current image of the sprite is 4*/
    IMAGE5, /*!<Current image of the sprite is 5*/
    IMAGE6, /*!<Current image of the sprite is 6*/
    IMAGE7, /*!<Current image of the sprite is 7*/
    IMAGE8, /*!<Current image of the sprite is 8*/
    IMAGE9  /*!<Current image of the sprite is 9*/
    }Sprite;

/**
 * @brief Updates the xpm on both fireworks1 and fireworks2, based on the current sprite,
 * will also increment sprite by 1 or if sprite is currently in the ninth one will go back to the first
 * @param fireworks1 Left side firework's struct
 * @param fireworks2 Right side firework's struct
 * @param sprite Pointer to the enumeration containing the last sprite to have been drawn on the screen
 */
void change_sprite(xpm* fireworks1, xpm* fireworks2, Sprite* sprite);

/**
 * @brief Updates the xpm containing the score, based on the points
 * @param number Struct containing the xpm to be updated
 * @param points Current score of the player to be updated
 */
void change_points(xpm* number, int points);

/**
 * @brief main menu function that controls the mouse movement on the screen, and checks if the mouse is on top of any of the buttons
 * @param pack pointer to the mouse packet
 * @return 0 if play button was pressed, 1 if exit button was pressed, -1 if none of the buttons were pressed
 */ 
int(main_menu)(struct packet *pack);

/**
 * @brief main game function which contains the driver receive loop and controls every device interruptions for the different game states
 */
void main_loop();

/**
 * @brief calculates and updates the rackets movements
 */ 
void racket_movement();

/**
 * @brief calculates and updates ball movement
 * @return 1 if player 1 scores, 2 if player 2 scores
 */ 
int ball_movement();

#endif

