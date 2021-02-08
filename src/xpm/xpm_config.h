#ifndef __XPM_CONFIG_H
#define __XPM_CONFIG_H

#include <stdbool.h>
#include <stdint.h>

 // a line containing XPM information (both the data and the pointer are constant)
typedef char const *const xpm_row_t;
  
 // ensure that neither the pointer nor the content of an XPM pixmap can be changed
 typedef xpm_row_t * const xpm_map_t;

/**
 * @brief This struct contains all the information about a xpm
*/
 typedef struct{
    void * xpm; /*!<Xpm of the image*/
    int16_t x;  /*!<X position*/
    int16_t y; /*!<Y position*/
    int16_t speed; /*!<Speed that the xpm moves in the screen*/
}xpm;

/**
 * @brief This struct contains all the information about a ball's xpm
*/
 typedef struct{
    void * xpm; /*!<Xpm of the image*/
    int16_t x;  /*!<X position*/
    int16_t y; /*!<Y position*/
    int16_t x_speed; /*!<Speed that the xpm moves in the x direction of the screen*/
    int16_t y_speed; /*!<Speed that the xpm moves in the y direction of the screen*/
}ball_xpm;

#endif
