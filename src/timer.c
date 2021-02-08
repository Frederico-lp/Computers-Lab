#include <lcom/lcf.h>
#include <stdint.h>
#include "i8254.h"
#include "timer.h"

int hook_id = TIMER0_IRQ;
unsigned int counter = 0;
////////mudei o counter nesta funçao

int (timer_set_frequency)(uint8_t timer, uint32_t freq) { 
  if ((timer < 0 || timer > 2) || (freq < 19 || freq > TIMER_FREQ))
  {
    printf("Error! Introduce a valid timer or a valid frequency!\n");
    return 1;
  }
  uint16_t newFreq = TIMER_FREQ / freq;
  uint8_t  newFreqLSB, newFreqMSB;
  util_get_LSB(newFreq, &newFreqLSB);
  util_get_MSB(newFreq, &newFreqMSB);
  uint8_t conf;
  if (timer_get_conf(timer,&conf) != OK){ return 1; }
  conf = (conf & 0x0F) | TIMER_LSB_MSB;
  switch(timer)
  {
    case 0:
      conf = conf | TIMER_SEL0;
      if (sys_outb(TIMER_CTRL, conf) != OK) {return 1;}
      if (sys_outb(TIMER_0, newFreqLSB) != OK) {return 1;}
      if (sys_outb(TIMER_0, newFreqMSB) != OK) {return 1;}
      return 0;
    case 1:
      conf = conf | TIMER_SEL1;
      if (sys_outb(TIMER_CTRL, conf) != OK) {return 1;}
      if (sys_outb(TIMER_1, newFreqLSB) != OK) {return 1;}
      if (sys_outb(TIMER_1, newFreqMSB) != OK) {return 1;}
      return 0;
    case 2:
      conf = conf | TIMER_SEL2;
      if (sys_outb(TIMER_CTRL, conf) != OK) {return 1;}
      if (sys_outb(TIMER_2, newFreqLSB) != OK) {return 1;}
      if (sys_outb(TIMER_2, newFreqMSB) != OK) {return 1;}
      return 0;    
  }
  return 1;
}


int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = (uint8_t) hook_id; //(uint8_t) hook_id
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) == OK)
  {
    printf("Timer subscribe was a sucess!\n");
    return 0;
  }
  else 
  {
    return 1;
  }
}

int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hook_id) == OK)
  {
    printf("Timer unsubscribe was a sucess\n");
    return 0;
  }
  else
  {
    return 1;
  }
  
}

void (timer_int_handler)() { 
  counter++;
  }

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if ( timer < 0 || timer > 2) { 
    printf("This timer doesn't exist. Insert a timer between 0, 1 or 2.\n");
    return 1;
  }

  uint8_t rbc = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_;

  if(sys_outb(TIMER_CTRL, rbc) == OK)
  {
    if(util_sys_inb(TIMER_0 + timer, st) == OK)
    {
      printf("Read was a sucess! SYS_INB() worked!\n");
      return 0;
    }
    else
    {
      printf("sys_inb() failed!\n");
      return 1;
    }

  }
  else
  {
    printf("ERROR : sysoutb failed!\n");
    return 1;
  }
  
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field)
{
  union timer_status_field_val val;
  uint8_t temp;
  if (field == tsf_all)
  {
    val.byte = st;
  }
  else if (field == tsf_initial)
  {
    temp = (st & (BIT(4) | BIT(5)));
    switch (temp)
    {
      case TIMER_LSB:
        val.in_mode = LSB_only;
        break;
      case TIMER_MSB:
        val.in_mode = MSB_only;
        break;
      case TIMER_LSB_MSB:
        val.in_mode = MSB_after_LSB;
        break;
      default:
        val.in_mode = INVAL_val;
    }
  }
  else if (field == tsf_mode)
  {
    val.count_mode = ((st & (BIT(1) | BIT(2) | BIT(3))));
    if((val.count_mode & BIT(2)) == BIT(2)){ //significa que o bit 2 esta a 1, logo o bit 3 nao interessa, sendo necessario "remove lo" (coloca lo a zero)
      val.count_mode = (val.count_mode & (BIT(1) | BIT(2)));
    }
    val.count_mode >>= 1;
  }
  else if (field == tsf_base)
  {
    val.bcd = st & BIT(0);
  }
  else
  {
    printf("Error! This field is not valid!\n");
    return 1;
  }

  if (timer_print_config(timer,field, val) == 0)
  {
    return 0;
  }
  else
  {
    printf("Error in timer_print_config function!\n");
    return 1;
  }
}
