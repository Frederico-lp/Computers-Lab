#ifndef __MOUSE_H
#define __MOUSE_H

#include <stdint.h>
#include <stdio.h>

/** @defgroup mouse mouse
 * @{
 *
 * Functions for controlling mouse device
 */

/**
 * @brief Handles the mouse interrupts
 */
void (mouse_ih)();

/**
 * @brief Subscribes mouse interrupts
 * @param bit_no Pointer that returns information about the subscription
 * @return 0 upon success, 1 otherwise
 */ 
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes mouse interrupts
 * @return 0 upon success, 1 otherwise
 */ 
int (mouse_unsubscribe_int)();

/**
 * @brief Updates the struct "pack" with the information from the mouse packet "vect"
 * @param vect The mouse packets to be processed
 * @param pack The struct to be updated with the mouse packets
*/
void (create_packages)(uint8_t *vect, struct packet *pack);

/**
 * @brief Sends a command to the mouse
 * @param cmd The command to be send to the mouse
 * @return 
*/
int (mouse_send_cmd)(uint8_t cmd);

/**
 * @brief Activates the mouse data reporting
 * @return 0 upon success, 1 otherwise
*/
int (mouse_enable_data_report2)();

/**
 * @brief Disables the mouse data reporting
 * @return 0 upon success, 1 otherwise
*/
int (mouse_disable_data_report2)();

/**
 * @brief Sets the mouse to remote mode
 * @return 0 upon success, 1 otherwise
*/
int (mouse_set_remote_mode)();

/**
 * @brief Tries to read unused data
*/
void (clean_output_buffer)();


/**
 * @brief Send data packet request to the mouse
 * @return 0 upon success, 1 otherwise
*/
int (mouse_read_data)();

/**
 * @brief Requests the mouse to send packets and if the mouse has packets to send the function will read the packets
 * @return 0 upon success, 1 otherwise 
*/
int (mouse_remote_reading)();

/**
 * @brief Analyse if the gesture done by the mouse is the correct one
 * @param mouse_packet The mouse packet to be processed 
 * @param x_len Minimum value on x axis movement
 * @param tolerance The tolerance in the mouse movement
*/
int (analyse_gesture)(struct packet *mouse_packet, uint8_t x_len, uint8_t tolerance);

#endif

