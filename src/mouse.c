#include <lcom/lcf.h>
#include "mouse.h"
#include "i8042.h"

static int hook_id = MOUSE_IRQ;
uint8_t mouse_scan, mouse_temp;

void (mouse_ih)()
{
    uint8_t val;
    int i = 0;
    while(i < 5)
    {
        if (util_sys_inb(STATUS_REGISTER,&val)){    //read-back cmd 
            printf("Reading of status register failed");
        }
        if((val & KBC_AUX) == 0){   //se o aux estiver a 0 entao n ha mouse data
            goto END;
        }
        if((val & KBC_OBF) == 0){  //se o OBF estiver a 0 não tem nada para ler, return nada
            goto END;
        }
        if ((val & (KBC_PAR_ERR | KBC_TO_ERR)) == 0 ){  //ve se n tem erros
            util_sys_inb(OUTPUT_BUFFER,&mouse_scan);
            return;
        }
        else{
            util_sys_inb(OUTPUT_BUFFER,&mouse_temp);  // se tiver erro le na mesma mas n guarda em local 
        }
        END:
        i++;
        tickdelay(micros_to_ticks(DELAY_US));
    }
}

int (mouse_read_data)()
{
    return mouse_send_cmd(READ_DATA);
}

int (mouse_remote_reading)()
{
    uint8_t val;
    int i = 0;
    while(i < 5)
    {
        if (util_sys_inb(STATUS_REGISTER,&val)){    //read-back cmd 
            printf("Reading of status register failed");
        }
        if((val & KBC_AUX) == 0){   //se o aux estiver a 0 entao n ha mouse data
            goto END;
        }
        if((val & KBC_OBF) == 0){  //se o OBF estiver a 0 não tem nada para ler, return nada
            goto END;
        }
        if ((val & (KBC_PAR_ERR | KBC_TO_ERR)) == 0 ){  //ve se n tem erros
            util_sys_inb(OUTPUT_BUFFER,&mouse_scan);
            return 0;
        }
        else{
            util_sys_inb(OUTPUT_BUFFER,&mouse_temp);  // se tiver erro le na mesma mas n guarda em local
        }
        END:
        i++;
    }
    return 1;
}

int (mouse_subscribe_int)(uint8_t *bit_no)
{
    *bit_no = hook_id;
    if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id)){
        return 1;
    }
  return 0;
}

int (mouse_unsubscribe_int)()
{
    if(sys_irqrmpolicy(&hook_id)){
        return 1;
    } 
    return 0;
}

void (create_packages)(uint8_t *vect, struct packet *pack)
{
    int i;
    for(i = 0;i < 3;i++){
        pack->bytes[i] = vect[i];
    }
    pack->rb = (pack->bytes[0] & RIGHT_BUTTON);

    pack->mb = (pack->bytes[0] & MIDDLE_BUTTON);

    pack->lb = (pack->bytes[0] & LEFT_BUTTON);


    pack->x_ov = (pack->bytes[0] & DELTA_X_OVERFLOW);

    pack->y_ov = (pack->bytes[0] & DELTA_Y_OVERFLOW);
    
    pack->delta_x = (uint16_t) vect[1];
    if(pack->bytes[0] & MSB_X_DELTA){
        pack->delta_x = 0xFF00 | vect[1];
    }
    pack->delta_y = (uint16_t)vect[2];
    if(pack->bytes[0] & MSB_Y_DELTA){
        pack->delta_y = 0xFF00 | vect[2];
    }
}

int (mouse_send_cmd)(uint8_t cmd){
    uint8_t val;
    int errorCounter = 0;
    int nackError = 0;
    do
    {
        if (util_sys_inb(STATUS_REGISTER,&val)){    //read-back cmd 
            return 1;
        }
        while( ((val & KBC_IBF) != 0) && (errorCounter < 5) ){
            util_sys_inb(STATUS_REGISTER, &val);
            tickdelay(micros_to_ticks(DELAY_US)); 
            errorCounter++;
        }
        errorCounter = 0;                
        if((val & KBC_IBF) == 0){ //checking if input buffer is empty
            sys_outb(INPUT_BUFFER_1, WRITE_BYTE_MOUSE);  //CMD to write command byte 
            //verificar novamente o status register para enviar o comando
            if (util_sys_inb(STATUS_REGISTER,&val))
                return 1;
            while( ((val & KBC_IBF) != 0) && (errorCounter < 5) ){
                util_sys_inb(STATUS_REGISTER, &val);
                tickdelay(micros_to_ticks(DELAY_US)); 
                errorCounter++;
            }
            if ((val & KBC_IBF) == 0){
                sys_outb(INPUT_BUFFER_2, cmd);// aqui escrevo o comando
                if (util_sys_inb(STATUS_REGISTER,&val))
                    return 1;
                while( ((val & KBC_OBF) == 0) && (errorCounter < 5) ){
                    util_sys_inb(STATUS_REGISTER, &val);
                    tickdelay(micros_to_ticks(DELAY_US)); 
                    errorCounter++;
                }
                if ((val & KBC_OBF) != 0)
                {
                    util_sys_inb(OUTPUT_BUFFER, &val);   //verificar se houve erros
                }        
            }
            else
            {
                return 1;
            }
            
            switch (val)
            {
                case ACK:
                    return 0;
                case NACK:
                    nackError++;
                    break; 
                case ERROR:
                    printf("second invalid byte error in send mouse cmd\n");
                    return 1;
                default:
                    return 0;
            }
        }
        errorCounter++;
    }while (errorCounter < 5 && nackError < 2);
    return 1;
}

int (mouse_enable_data_report2)(){
    return mouse_send_cmd(ENABLE_DATA_REPORTING);
}

int (mouse_disable_data_report2)(){
    return mouse_send_cmd(DISABLE_DATA_REPORTING);
}

int (mouse_set_remote_mode())
{
    return mouse_send_cmd(REMOTE_MODE);
}


void (clean_output_buffer)(){
    uint8_t temp;
    if (util_sys_inb(STATUS_REGISTER,&temp)){    //read-back cmd 
        printf("Reading of status register failed");
    }
    if((temp & KBC_OBF) == 1){  
        util_sys_inb(OUTPUT_BUFFER,&temp);
    }

}

typedef enum {INIT, DRAWUP, MIDPOINT, DRAWDOWN, COMP} state_t;

int analyse_gesture(struct packet *mouse_packet, uint8_t x_len, uint8_t tolerance)
{
    static state_t st = INIT;
    static int x_mov = 0;
    static int y_mov = 0;
    switch(st)
    {
        case INIT:
            if (mouse_packet->lb && !(mouse_packet->mb) && !(mouse_packet->rb))
            {
                st = DRAWUP; //desenhando a primeira linha
            }
            break;
        case DRAWUP:
            if(mouse_packet->mb || mouse_packet->rb)
            {
                st = INIT;
                x_mov = 0;
                y_mov = 0;
                break;
            }
            else if (!mouse_packet->lb)
            { //botao esquerda foi largado
                if (x_mov >= x_len && abs(y_mov/x_mov)> 1) //condicao para terminar primeira linha, movimento em x maior ou igual que o pedido(x_len) e declive da reta maior que 1
                {
                    st = MIDPOINT;
                }
                else
                {
                    st = INIT;
                }
                x_mov = 0;
                y_mov = 0;
                break;
            }
            else
            {
                if((mouse_packet->delta_x < 0 && abs(mouse_packet->delta_x) > tolerance) || (mouse_packet->delta_y < 0 && abs(mouse_packet->delta_y) > tolerance))
                { //verificacao da tolerancia
                    st = INIT;
                    x_mov = 0;
                    y_mov = 0;
                }
                else
                {
                    x_mov += mouse_packet->delta_x;
                    y_mov += mouse_packet->delta_y;
                }
                break;
            }
        case MIDPOINT:
            if (mouse_packet->rb && !(mouse_packet->lb) && !(mouse_packet->mb))
            {
                st = DRAWDOWN; //desenhando a segunda linha
            }
            else if(mouse_packet->lb)
            { //se botao premido for o esquerdo significa que estara a comecar o desenho da linha esqueda de novo sem ir para o inicio
                st = DRAWUP;
                x_mov = mouse_packet->delta_x;
                y_mov = mouse_packet->delta_y;
            }
            else if (mouse_packet->mb 
            || abs(mouse_packet->delta_x) > tolerance || abs(mouse_packet->delta_y) > tolerance)
            {
                st = INIT;
            }
            break;
        case DRAWDOWN:
            if(mouse_packet->mb)
            {
                st = INIT;
                x_mov = 0;
                y_mov = 0;
                break;
            }
            else if (mouse_packet->lb)
            {
                st = DRAWUP;
                x_mov = mouse_packet->delta_x;
                y_mov = mouse_packet->delta_y;
            }
            else if (!mouse_packet->rb)
            { //botao direito foi largado
                if (x_mov >= x_len && abs(y_mov/x_mov)> 1 && (y_mov/x_mov) < 0) //condicao para terminar segunda linha, movimento em x maior ou igual que o pedido(x_len) e declive absoluto da reta maior que 1 e menor que 0
                {
                    st = COMP;
                }
                else
                {
                    st = INIT;
                }
                x_mov = 0;
                y_mov = 0;
                break;
            }
            else
            {
                if((mouse_packet->delta_x < 0 && abs(mouse_packet->delta_x) > tolerance) || (mouse_packet->delta_y > 0 && mouse_packet->delta_y > tolerance))
                { //verificacao da tolerancia
                    st = INIT;
                    x_mov = 0;
                    y_mov = 0;
                }
                else
                {
                    x_mov += mouse_packet->delta_x;
                    y_mov += mouse_packet->delta_y;
                }
                break;
            }
        default:
            break;
    }
    if (st == COMP)
        return 0;
    else
        return 1;
}


