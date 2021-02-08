#include <lcom/lcf.h>
#include "i8042.h"

static int hook_id = KEYBOARD_IRQ;
uint8_t kbd_scan, kbd_temp;


void (kbc_ih)()
{
  // The handler should read the status from the status register, to check if there was some communications error, and to read the scancode byte from the output buffer.
  uint8_t val;
  if (util_sys_inb(STATUS_REGISTER,&val)){    //status
    printf("Reading of status register failed");
    return;
  }
  if((val & KBC_OBF) == 0)
  { //se o OBF estiver a 0 não tem nada para ler, return nada
    return;
    printf("n tem nada para ler");
  }
  if ((val & (KBC_PAR_ERR | KBC_TO_ERR)) == 0 )
  {  //ve se n tem erros
    util_sys_inb(OUTPUT_BUFFER,&kbd_scan);
  }
  else 
  {
    util_sys_inb(OUTPUT_BUFFER,&kbd_temp); // se tiver erro le na mesma mas n guarda em local util, para fazer o bit de ouptut buffer voltar a 
    printf("Reading of output buffer failed");
    return;
  }
}

int kbc_polling()
{
  uint8_t val;
  if (util_sys_inb(STATUS_REGISTER,&val)){    //status
    printf("Reading of status register failed");
    return 0;
  }
  if((val & KBC_OBF) == 0){  //se o OBF estiver a 0 não tem nada para ler, return nada
    return 0;
  }
  if ((val & (KBC_PAR_ERR | KBC_TO_ERR)) == 0 ){  //ve se n tem erros
    util_sys_inb(OUTPUT_BUFFER,&kbd_scan);
    return 1;
  }
  else {
    util_sys_inb(OUTPUT_BUFFER,&kbd_temp); // se tiver erro le na mesma mas n guarda em local util
    printf("Reading of output buffer failed");
    return 0;
  }
  return 0;
}


int (keyboard_subscribe_int)(uint8_t *bit_no)
{
  //subscribe an interrupt, ou notificação?
  *bit_no = hook_id;
  if(sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != 0){
    printf("keyboard_subscribe_int failed");
    return 1;

  }
  return 0;
}


int (keyboard_unsubscribe_int)()
{
   if(sys_irqrmpolicy(&hook_id) != 0){
    printf("keyboard_unsubscribe_int failed");
    return 1;
  } 
  return 0;
}

void make_or_break(uint8_t byte, uint8_t *size, bool *make)
{
  // ver se começa em E0?
  if(byte == 0xe0){
    *size = 2;
  }
  else {*size = 1;}
  if((byte & 0x80)){
    *make = false;  //false indica que é break-code
  }
  else {*make = true;}
}



int keyboard_interrupt_enable()
{
  // 
  //testar se o input buffer esta cheio, se estiver esperar
  //escrever command em 0x60 para poder escrever command byte 
  //writing KBC command byte
  // falta ler o cmd byte para o restaurar dps
  uint8_t val, reg;
  int i=0;
  if (util_sys_inb(STATUS_REGISTER, &val)){    //read-back cmd 
    printf("Reading of status register failed");
  }
  while( ((val & KBC_IBF) != 0) && (i < 5) ){
    tickdelay(micros_to_ticks(DELAY_US)); 
    i++;
  }
  if((val & KBC_IBF) == 0){ //checking if input buffer is empty
    sys_outb(INPUT_BUFFER_1, READ_CMD_BYTE);
    util_sys_inb(OUTPUT_BUFFER, &reg);
    sys_outb(INPUT_BUFFER_1, WRITE_CMD_BYTE);  //CMD to write command byte
    reg = reg | KB_EN_INTERRUPT_CMD;
    sys_outb(INPUT_BUFFER_2, reg);
    return 0;
  }
  return 1;
}

