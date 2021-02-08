#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <stdint.h>
#include <unistd.h>

/** @defgroup keyboard keyboard
 * @{
 *
 * Functions for controlling keyboard device
 */

/**
 * @brief Handles the Keybpard interrupts
 */
void (kbc_ih)();

/**
 * @brief Operates the keyboard in polling mode
 */
int kbc_polling();

/**
 * @brief Subscribes the keyboard interrupts
 * @param bit_no Pointer that returns information about the subscription
 * @return 0 upon success, 1 otherwise
 */ 
int (keyboard_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes the keyboard interrupts
 * @return 0 upon success, 1 otherwise
 */ 
int (keyboard_unsubscribe_int)();

/**
 * @brief Check if the byte passed is either 1 byte or 2 bytes size, and if it is a makecode or a breakcode
 * @param byte Byte to be processed
 * @param size Will be updated with 1 or 2, depending on the size of the byte
 * @param make Will be updated with true if the byte is a makecode, false if the byte is a breakcode
 * @return 0 upon success, 1 otherwise
 */  
void (make_or_break)(uint8_t byte, uint8_t *size, bool *make);

/**
 * @brief Activates the keyboard interrupts
 * @return 0 upon success, 1 otherwise
 */  
int (keyboard_interrupt_enable)();

#endif /* __KEYBOARD_H */
