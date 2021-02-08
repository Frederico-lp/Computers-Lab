#include <lcom/lcf.h>
#include <stdint.h>
#include "rtc.h"


int rtc_hook_id = RTC_IRQ;


int (rtc_subscribe_int)(uint8_t *bit_no)
{
    *bit_no = rtc_hook_id;
    if(sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &rtc_hook_id)){
        //qual das policies é q escolho?
        return 1;
    }
  return 0;
}



int (rtc_unsubscribe_int)()
{
    if(sys_irqrmpolicy(&rtc_hook_id)){
        return 1;
    } 
    return 0;
}


void (rtc_ih)()
{
    int cause;
    uint32_t regA;
    sys_outb(RTC_ADDR_REG, RTC_REG_C);
    cause = sys_inb(RTC_DATA_REG, &regA);
}

int rtc_read(uint8_t address, uint8_t *data)
{
    if(sys_outb(RTC_ADDR_REG, address)){
        return 1;
    }
    if(util_sys_inb(RTC_DATA_REG, data)){
        return 1;
    }
    return 0;

}

int rtc_write(uint8_t address, uint8_t data)
{
    if(sys_outb(RTC_ADDR_REG, address)){
        return 1;
    }
    if(sys_outb(RTC_DATA_REG, data)){
        return 1;
    }
    return 0;
    
}


int rtc_read_hour()
{
    uint8_t data;
    if(rtc_read(RTC_HOURS, &data)){
        return 1;
    }
    return bcd_to_decimal(data);

}

int bcd_to_decimal(uint16_t bcd)
{ //16 chega n chega?
    return ( ((bcd >> 4) * 10) + (bcd & 0xF) );
}

int day_or_night()
{
    int hour;
    hour = rtc_read_hour();
    if(hour < 8 || hour > 19){
        return 1; //night
    }
    else {
        return 0; //dia
    }
}


