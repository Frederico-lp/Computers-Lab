#ifndef _RTC_H
#define _RTC_H

/** @defgroup rtc rtc
 * @{
 *
 * Functions for controlling keyboard device
 */

#define RTC_IRQ 8

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define RTC_REG_C 0x0c

#define RTC_HOURS 4

#define RTC_REGISTER_A 10
#define RTC_REGISTER_B 11
#define RTC_REGISTER_C 12
#define RTC_REGISTER_D 13

#define RTC_24H 0x2
//caso esteja no modo 12 horas carregar isto no registo B

/**
 * @brief Subscribes rtc interrupts
 * @param bit_no Pointer that returns information about the subscription
 * @return 0 upon success, 1 otherwise
 */ 
int (rtc_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes rtc interrupts
 * @return 0 upon success, 1 otherwise
 */ 
int (rtc_unsubscribe_int)();

/**
 * @brief Handles the rtc interrupts
 */
void (rtc_ih)();

/**
 * @brief Reads data from a rtc register
 * @param address Adress from where we read the data
 * @param data Data read from the register
 * @return 0 upon success, 1 otherwise
 */ 
int rtc_read(uint8_t address, uint8_t *data);


/**
 * @brief Writes data in a rtc register
 * @param address Where we write the data
 * @param data Data that will be written in the register
 * @return 0 upon success, 1 otherwise
 */ 
int rtc_write(uint8_t address, uint8_t data);

/**
 * @brief Reads current hour
 * @return Current hour upon success, 1 otherwise
 */ 
int rtc_read_hour();

/**
 * @brief Converts number from bcd to decimal
 * @param bcd Number in bcd that we want to convert
 * @return Decimal number
 */ 
int bcd_to_decimal(uint16_t bcd);

/**
 * @brief Determines if is currently day or night(assuming day from 8h to 19h)
 * @return 0 if day, 1 otherwise
 */ 
int day_or_night();

#endif
