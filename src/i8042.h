#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042
 */


#define KEYBOARD_IRQ 1
#define MOUSE_IRQ 12
#define STATUS_REGISTER 0x64
#define OUTPUT_BUFFER 0x60
#define INPUT_BUFFER_1 0x64
#define INPUT_BUFFER_2 0x60

#define KBC_OBF 0x01
#define KBC_IBF 0x02
#define KBC_PAR_ERR 0x80
#define KBC_TO_ERR 0x40
#define ESC 0x81
#define DELAY_US 20000
#define WRITE_CMD_BYTE 0x60
#define RETURN_CMD_BYTE 0x60
#define KB_EN_INTERRUPT_CMD 0x01
#define READ_CMD_BYTE 0x20
#define KBC_AUX 0x20

///// MOUSE 3-BYTE DATA PACKAGE
#define RIGHT_BUTTON BIT(1)
#define MIDDLE_BUTTON BIT(2)
#define LEFT_BUTTON BIT(0)
#define DELTA_X_OVERFLOW BIT(6)
#define DELTA_Y_OVERFLOW BIT(7)
#define WRITE_BYTE_MOUSE 0xD4
#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC
#define STREAM_MODE 0xEA
#define DISABLE_DATA_REPORTING 0xF5
#define ENABLE_DATA_REPORTING 0xF4
#define REMOTE_MODE 0xF0
#define READ_DATA 0xEB
#define MSB_X_DELTA BIT(4)
#define MSB_Y_DELTA BIT(5)

#define DOWN_ARROW 0x50
#define UP_ARROW 0x48
#define DOWN_ARROW_BREAK 0xD0
#define UP_ARROW_BREAK 0xC8
#define LEFT_ARROW 0x4B
#define RIGHT_ARROW 0x4D

#define W_MAKE 0x11
#define S_MAKE 0x1F
#define W_BREAK 0x91
#define S_BREAK 0x9F

#endif
