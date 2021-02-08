#ifndef __VIDEO_H
#define __VIDEO_H

/** @defgroup video video
 * @{
 *
 * Functions for using graphics card
 */

/**
 * @brief exits the function when esc key is released, stays in loop otherwise
 * @return 0 upon success, 1 otherwise
 */ 
int run_until_esc();

/**
 * @brief draws a xpm in video memory
 * @param xpm Xpm to be draw in the screen
 * @param x X position of the upper left corner of xpm
 * @param y Y position of the upper left corner of xpm
 */ 
void xpm_draw(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief copies video memory to a second buffer that draws image on the screen
 */ 
void buffer_cpy();

#endif
