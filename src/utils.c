#include <lcom/lcf.h>
#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) 
{
  val = val & 0x00FF;
  *lsb = (uint8_t)val;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb)
{
  val = val & 0xFF00;
  val = val >> 8;
  *msb = (uint8_t)val;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value)
{
  uint32_t rbc_status;
  if(sys_inb(port, &rbc_status) == OK)
  {
    *value = (uint8_t) rbc_status;
    return 0;
  }
  else
  {
    return 1;
  }
  return 1;  
}
