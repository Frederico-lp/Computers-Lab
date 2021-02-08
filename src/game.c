#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "i8042.h"
#include "game.h"
#include "video_macros.h"
#include "video.h"
#include "keyboard.h"
#include "timer.h"
#include "mouse.h"
#include "rtc.h"
#include "xpm/ball_day.h"
#include "xpm/ball_night.h"
#include "xpm/cursor_day.h"
#include "xpm/cursor_night.h"
#include "xpm/exit_button_day.h"
#include "xpm/exit_button_night.h"
#include "xpm/0_day.h"
#include "xpm/0_night.h"
#include "xpm/1_day.h"
#include "xpm/1_night.h"
#include "xpm/2_day.h"
#include "xpm/2_night.h"
#include "xpm/3_day.h"
#include "xpm/3_night.h"
#include "xpm/4_day.h"
#include "xpm/4_night.h"
#include "xpm/5_day.h"
#include "xpm/5_night.h"
#include "xpm/6_day.h"
#include "xpm/6_night.h"
#include "xpm/7_day.h"
#include "xpm/7_night.h"
#include "xpm/8_day.h"
#include "xpm/8_night.h"
#include "xpm/9_day.h"
#include "xpm/9_night.h"
#include "xpm/P_day.h"
#include "xpm/P_night.h"
#include "xpm/O_day.h"
#include "xpm/O_night.h"
#include "xpm/N_day.h"
#include "xpm/N_night.h"
#include "xpm/G_day.h"
#include "xpm/G_night.h"
#include "xpm/mid_line_day.h"
#include "xpm/mid_line_night.h"
#include "xpm/options_button_day.h"
#include "xpm/options_button_night.h"
#include "xpm/play_button_day.h"
#include "xpm/play_button_night.h"
#include "xpm/racket_day.h"
#include "xpm/racket_night.h"
#include "xpm/player1_win_day.h"
#include "xpm/player1_win_night.h"
#include "xpm/player2_win_day.h"
#include "xpm/player2_win_night.h"
#include "xpm/sprite1.h"
#include "xpm/sprite2.h"
#include "xpm/sprite3.h"
#include "xpm/sprite4.h"
#include "xpm/sprite5.h"
#include "xpm/sprite6.h"
#include "xpm/sprite7.h"
#include "xpm/sprite8.h"
#include "xpm/sprite9.h"

extern uint8_t kbd_scan, kbd_temp;
extern uint8_t mouse_scan, mouse_temp;
extern unsigned int counter;

Game_State game_state = MAINMENU;
extern Time_Day time_of_day;
Sprite sprite_fireworks;

//variables for rackets movement
bool move_up1 = 0, move_down1 = 0, move_up2 = 0, move_down2 = 0;
bool flag = true;


int points_pl1 = 0, points_pl2 = 0;
int winner;
int fr_rate = 30;

xpm cursor;
xpm P_letter;
xpm O_letter;
xpm N_letter;
xpm G_letter;
xpm play_button;
xpm options_button;
xpm exit_button;
xpm racket1;
xpm racket2;
xpm mid_line;
xpm points_player1;
xpm points_player2;
ball_xpm ball;
xpm winner_background;
xpm fireworks1;
xpm fireworks2;


void change_sprite(xpm* fireworks1, xpm* fireworks2, Sprite* sprite)
{
    switch ((*sprite))
    {
        case IMAGE1:
            fireworks1->xpm = (xpm_row_t**)&sprite2;
            fireworks2->xpm = (xpm_row_t**)&sprite2;
            (*sprite) = IMAGE2;
            break;
        case IMAGE2:
            fireworks1->xpm = (xpm_row_t**)&sprite3;
            fireworks2->xpm = (xpm_row_t**)&sprite3;
            (*sprite) = IMAGE3;
            break;    
        case IMAGE3:
            fireworks1->xpm = (xpm_row_t**)&sprite4;
            fireworks2->xpm = (xpm_row_t**)&sprite4;
            (*sprite) = IMAGE4;
            break;  
        case IMAGE4:
            fireworks1->xpm = (xpm_row_t**)&sprite5;
            fireworks2->xpm = (xpm_row_t**)&sprite5;
            (*sprite) = IMAGE5;
            break;
        case IMAGE5:
            fireworks1->xpm = (xpm_row_t**)&sprite6;
            fireworks2->xpm = (xpm_row_t**)&sprite6;
            (*sprite) = IMAGE6;
            break;
        case IMAGE6:
            fireworks1->xpm = (xpm_row_t**)&sprite7;
            fireworks2->xpm = (xpm_row_t**)&sprite7;
            (*sprite) = IMAGE7;
            break;
        case IMAGE7:
            fireworks1->xpm = (xpm_row_t**)&sprite8;
            fireworks2->xpm = (xpm_row_t**)&sprite8;
            (*sprite) = IMAGE8;
            break;  
        case IMAGE8:
            fireworks1->xpm = (xpm_row_t**)&sprite9;
            fireworks2->xpm = (xpm_row_t**)&sprite9;
            (*sprite) = IMAGE9;
            break;
        case IMAGE9:
            fireworks1->xpm = (xpm_row_t**)&sprite1;
            fireworks2->xpm = (xpm_row_t**)&sprite1;
            (*sprite) = IMAGE1;
            break;                                  
        default:
            break;
    }
}

void change_points(xpm* number, int points)
{
    switch (points)
    {
    case 1:
        if(time_of_day == DAY)
            number->xpm = (xpm_row_t**)&number1_day;
        else
            number->xpm = (xpm_row_t**)&number1_night;
        break;
    case 2:
        if(time_of_day == DAY)
            number->xpm = (xpm_row_t**)&number2_day;
        else
            number->xpm = (xpm_row_t**)&number2_night;
        break;    
    case 3:
        if(time_of_day == DAY)
            number->xpm = (xpm_row_t**)&number3_day;
        else
            number->xpm = (xpm_row_t**)&number3_night;
        break;
    case 4:
        if(time_of_day == DAY)
            number->xpm = (xpm_row_t**)&number4_day;
        else
            number->xpm = (xpm_row_t**)&number4_night;
        break;
    case 5:
        if(time_of_day == DAY)
            number->xpm = (xpm_row_t**)&number5_day;
        else
            number->xpm = (xpm_row_t**)&number5_night;
        break;
    case 6:
        if(time_of_day == DAY)
            number->xpm = (xpm_row_t**)&number6_day;
        else
            number->xpm = (xpm_row_t**)&number6_night;
        break;
    case 7:
        if(time_of_day == DAY)
            number->xpm = (xpm_row_t**)&number7_day;
        else
            number->xpm = (xpm_row_t**)&number7_night;
        break;
    case 8:
        if(time_of_day == DAY)
            number->xpm = (xpm_row_t**)&number8_day;
        else
            number->xpm = (xpm_row_t**)&number8_night;
        break;
    case 9:
        if(time_of_day == DAY)
            number->xpm = (xpm_row_t**)&number9_day;
        else
            number->xpm = (xpm_row_t**)&number9_night;
        break;                           
    default:
        break;
    }
}

int(main_menu)(struct packet *pack)
{
    if (time_of_day == DAY)
        vg_draw_rectangle(cursor.x, cursor.y, 10, 15, 63);
    else
        vg_draw_rectangle(cursor.x, cursor.y, 10, 15, 0);
    if (cursor.x >= 392 && cursor.x <= 622 && cursor.y >= 355 && cursor.y <= 425) //rato esta por cima do botao play
    {
        if (pack->lb)
            return 0;
        xpm_draw(play_button.xpm, play_button.x, play_button.y);
    }
    else if (cursor.x >= 392 && cursor.x <= 622 && cursor.y >= 387 && cursor.y <= 585) //rato esta por cima do botao exit
    {
        if (pack->lb)
            return 1;
        xpm_draw(exit_button.xpm, exit_button.x, exit_button.y);
    }
    else if (cursor.x >= 300 && cursor.x <= 380 && cursor.y >= 95 && cursor.y <= 230) //rato esta por cima da letra P
    {
        xpm_draw(P_letter.xpm, P_letter.x, P_letter.y);
    }
    else if (cursor.x >= 410 && cursor.x <= 490 && cursor.y >= 95 && cursor.y <= 230) //rato esta por cima da letra O
    {
        xpm_draw(O_letter.xpm, O_letter.x, O_letter.y);
    }
    else if (cursor.x >= 520 && cursor.x <= 600 && cursor.y >= 95 && cursor.y <= 230) //rato esta por cima da letra N
    {
        xpm_draw(N_letter.xpm, N_letter.x, N_letter.y);
    }
    else if (cursor.x >= 630 && cursor.x <= 710 && cursor.y >= 95 && cursor.y <= 230) //rato esta por cima da letra G
    {
        xpm_draw(G_letter.xpm, G_letter.x, G_letter.y);
    }
    cursor.x += cursor.speed*pack->delta_x;
    cursor.y -= cursor.speed*pack->delta_y;
    if (cursor.x > 1014) //limite a direita (1024-10)
        cursor.x = 1014;
    if (cursor.x < 0) // limite a esquerda
        cursor.x = 0;   
    if (cursor.y > 753) //limite em baixo (768-15)
        cursor.y = 753;
    if (cursor.y < 0) //limite em cima
        cursor.y = 0;     
    xpm_draw(cursor.xpm, cursor.x, cursor.y);
    buffer_cpy();
    return -1;
}

void main_loop()
{
    if(day_or_night() == 0)
    {
        time_of_day = DAY;
    }
    else if(day_or_night() == 1)
    {
        time_of_day = NIGHT;
    }

    if (time_of_day == DAY)
    {
        cursor.xpm = (xpm_row_t**)&cursor_day;
        P_letter.xpm = (xpm_row_t**)&P_day;
        O_letter.xpm = (xpm_row_t**)&O_day;
        N_letter.xpm = (xpm_row_t**)&N_day;
        G_letter.xpm = (xpm_row_t**)&G_day;
        play_button.xpm = (xpm_row_t**)&play_button_day;
        exit_button.xpm = (xpm_row_t**)&exit_button_day;
        points_player1.xpm = (xpm_row_t**)&number0_day;
        points_player2.xpm = (xpm_row_t**)&number0_day;
        mid_line.xpm = (xpm_row_t**)&mid_line_day;
        racket1.xpm = (xpm_row_t**)&racket_day;
        racket2.xpm = (xpm_row_t**)&racket_day;
    }
    else
    {
        cursor.xpm = (xpm_row_t**)&cursor_night;
        P_letter.xpm = (xpm_row_t**)&P_night;
        O_letter.xpm = (xpm_row_t**)&O_night;
        N_letter.xpm = (xpm_row_t**)&N_night;
        G_letter.xpm = (xpm_row_t**)&G_night;
        play_button.xpm = (xpm_row_t**)&play_button_night;
        exit_button.xpm = (xpm_row_t**)&exit_button_night;
        points_player1.xpm = (xpm_row_t**)&number0_night;
        points_player2.xpm = (xpm_row_t**)&number0_night;
        mid_line.xpm = (xpm_row_t**)&mid_line_night;
        racket1.xpm = (xpm_row_t**)&racket_night;
        racket2.xpm = (xpm_row_t**)&racket_night; 
    }
    
    fireworks1.xpm = (xpm_row_t**)&sprite1;
    fireworks2.xpm = (xpm_row_t**)&sprite1;

    cursor.x = 500;
    cursor.y = 500;
    cursor.speed = 1;
    P_letter.x = 310;
    P_letter.y = 110;
    O_letter.x = 420;
    O_letter.y = 110;
    N_letter.x = 530;
    N_letter.y = 110;
    G_letter.x = 640;
    G_letter.y = 110;
    play_button.x = 402;
    play_button.y = 370;
    options_button.x = 402;
    options_button.y = 450;
    exit_button.x = 402;
    exit_button.y = 530;
    points_player1.x = 350;
    points_player1.y = 90;
    points_player2.x = 600;
    points_player2.y = 90;
    mid_line.x = 505;
    mid_line.y = 0;
    racket1.x = 15;
    racket1.y = 370;
    racket2.x = 985;
    racket2.y = 370;
    ball.x = 505;
    ball.y = 378;
    winner_background.x = 0;
    winner_background.y = 0;
    fireworks1.x = 50;
    fireworks1.y = 468;
    fireworks2.x = 674;
    fireworks2.y = 468;

    
    vg_draw_rectangle(0,0,1024,768,63);
    xpm_draw((xpm_row_t*)P_letter.xpm, P_letter.x, P_letter.y);
    xpm_draw((xpm_row_t*)O_letter.xpm, O_letter.x, O_letter.y);
    xpm_draw((xpm_row_t*)N_letter.xpm, N_letter.x, N_letter.y);
    xpm_draw((xpm_row_t*)G_letter.xpm, G_letter.x, G_letter.y);
    xpm_draw((xpm_row_t*)play_button.xpm, play_button.x, play_button.y);
    xpm_draw((xpm_row_t*)exit_button.xpm, exit_button.x, exit_button.y);
    buffer_cpy();

    int neg_or_pos = rand() % 2;
    if(neg_or_pos)
        ball.x_speed = 6;
    else 
        ball.x_speed = -6;

    ball.y_speed = rand() % 17 - 8;
    int points;

    uint8_t bit_no_timer;
    uint8_t bit_no_keyboard;
    uint8_t bit_no_mouse;
    int mouseCount = 0;
    int counter_sprite = 0;
    //int keyboard_pack_size = 0;
    uint32_t irq_set_mouse;
    uint32_t irq_set_timer;
    uint32_t irq_set_keyboard;
    struct packet pack;
    uint8_t vect[3];
    int ipc_status, r;
    message msg;

    if(mouse_enable_data_report2()){
         printf("mouse_enable_data_report failed\n");
        return;
    }
    if (timer_subscribe_int(&bit_no_timer) != 0){ 
        printf("timer_subscribe_int failed");
        return;
    }
    if (keyboard_subscribe_int(&bit_no_keyboard) != 0){
        printf("keyboard_susbcribe_int failed");
        return;
    }
    if (mouse_subscribe_int(&bit_no_mouse) != 0){
        printf("keyboard_susbcribe_int failed");
        return;
    }

    irq_set_mouse = BIT(bit_no_mouse);
    irq_set_timer = BIT(bit_no_timer);
    irq_set_keyboard = BIT(bit_no_keyboard);

    while (game_state != EXIT)
    {
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { 
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (game_state == MAINMENU)
                    { 
                        if (msg.m_notify.interrupts & irq_set_mouse)
                        { 
                            mouse_ih();
                            if (mouseCount == 0 && (mouse_scan & BIT(3)))
                            {
                                vect[mouseCount] = mouse_scan;
                                mouseCount++;
                            }
                            else if (mouseCount == 1 || mouseCount == 2)
                            {
                                vect[mouseCount] = mouse_scan;
                                mouseCount++;
                            }
                            if(mouseCount == 3)
                            {
                                create_packages(vect, &pack);
                                mouseCount = 0;
                                unsigned int state = main_menu(&pack);
                                if (state == 0)
                                {
                                    game_state = GAME_BACKGROUNG;
                                    if(mouse_disable_data_report2() !=0)
                                    {
                                        printf("mouse_disable_data_report failed\n");
                                        return;
                                    }
                                    clean_output_buffer();
                                }
                                if (state == 1)
                                {
                                    game_state = EXIT;
                                }          
                            }
                        }
                        break;
                    }
                    
                    if (game_state == GAME_BACKGROUNG)
                    {
                        ball.x = 505;
                        ball.y = 378;
                        vg_draw_rectangle(0,0,1024,768,63);
                        xpm_draw((xpm_row_t*)points_player1.xpm,points_player1.x, points_player1.y);
                        xpm_draw((xpm_row_t*)points_player2.xpm, points_player2.x, points_player2.y);
                        xpm_draw((xpm_row_t*)mid_line.xpm, mid_line.x, mid_line.y);
                        xpm_draw((xpm_row_t*)racket1.xpm, racket1.x, racket1.y);
                        xpm_draw((xpm_row_t*)racket2.xpm, racket2.x, racket2.y);
                        buffer_cpy();
                        sprite_fireworks = IMAGE1;
                        game_state = GAME;
                    }
                    if(game_state == GAME)
                    {
                        //bool flag = false;
                        if(kbd_scan == ESC)
                            return;
                        if (msg.m_notify.interrupts & irq_set_keyboard)
                        { 
                            kbc_ih();
                            if(kbd_scan != 0xe0)
                            {
                                racket_movement();

                            }
                        }
                        if (msg.m_notify.interrupts & irq_set_timer)
                        { 
                            timer_int_handler();
                            if( (counter % (60 / fr_rate)) == 0 ){
                                if((move_down1 || move_down2 || move_up1 || move_up2)){
                                    // se flag == true entao nao foi atualizada a posiçao da raquete neste ciclo
                                    if(flag)
                                        racket_movement();
                                }
                                points = ball_movement();
                            
                                if(points){
                                    if( points == 1)
                                    { 
                                        change_points(&points_player1, ++points_pl1);
                                        points = 0;
                                    }
                                    if(points == 2)
                                    {
                                        change_points(&points_player2, ++points_pl2);
                                        points = 0;
                                    }
                                    //new y speed and reset x speed
                                    neg_or_pos = rand() % 2;
                                    if(neg_or_pos)
                                        ball.x_speed = 6;
                                    else
                                        ball.x_speed = -6;
                                    ball.y_speed = rand() % 17 - 8;
                                    sleep(1);
                                    game_state = GAME_BACKGROUNG;
                                }
                                flag = true;
                                buffer_cpy();

                                
                            }
                        }                        
                        if(points_pl1 > 5 || points_pl2 > 5)
                        {
                            if (time_of_day == DAY && points_pl1 > 5)
                                winner_background.xpm = (xpm_row_t**)&player1_win_day;
                            if (time_of_day == NIGHT && points_pl1 > 5)
                                winner_background.xpm = (xpm_row_t**)&player1_win_night;
                            if (time_of_day == DAY && points_pl2 > 5)
                                winner_background.xpm = (xpm_row_t**)&player2_win_day;
                            if (time_of_day == NIGHT && points_pl2 > 5)
                                winner_background.xpm = (xpm_row_t**)&player2_win_night;
                            xpm_draw(winner_background.xpm, winner_background.x, winner_background.y);
                            buffer_cpy();
                            sprite_fireworks = IMAGE1;
                            game_state = WINNER;
                        }
                    }
                    if(game_state == WINNER)
                    {       
                        if (msg.m_notify.interrupts & irq_set_timer)
                        { 
                            timer_int_handler();
                            if( (counter % (60 / fr_rate)) == 0 )
                            {
                                ++counter_sprite;
                                if (counter_sprite == 5)
                                {
                                    vg_draw_rectangle(fireworks1.x, fireworks1.y,300,300,63);
                                    vg_draw_rectangle(fireworks2.x, fireworks2.y,300,300,63);
                                    printf("estou aqui\n");
                                    if (sprite_fireworks == IMAGE1)
                                    {
                                        sleep(2);
                                        printf("sleep\n");
                                    }
                                    xpm_draw(fireworks1.xpm, fireworks1.x, fireworks1.y);
                                    xpm_draw(fireworks2.xpm, fireworks2.x, fireworks2.y);
                                    change_sprite(&fireworks1, &fireworks2, &sprite_fireworks);
                                    printf("change_sprite\n");
                                    buffer_cpy();
                                    counter_sprite = 0;
                                }
                            }

                        }
                        if (msg.m_notify.interrupts & irq_set_keyboard)
                        { 
                            kbc_ih();
                            if(kbd_scan == ESC)
                            {
                                game_state = EXIT;
                            }
                        }
                    }
                default:
                    break;
            } 
        }
        else{
        } 
    }
    if(mouse_unsubscribe_int() != 0)
    {
        printf("keyboard_unsubscribe_int failed");
        return;
    }

    if(keyboard_unsubscribe_int() != 0){
        printf("keyboard_unsubscribe_int failed");
        return;
    }
    if (timer_unsubscribe_int() != 0){
        printf("keyboard_unsubscribe_int failed");
        return;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void racket_movement()
{
    int racket_width = 15;
    int racket_height = 80;

    //racket parameters
    racket1.speed = 7;
    racket2.speed = 7;
    if(kbd_scan == UP_ARROW_BREAK)
    {
        move_up2 = 0;
    }
    else if(kbd_scan == DOWN_ARROW_BREAK)
    {
        move_down2 = 0;
    }
    else if(kbd_scan == W_BREAK)
    {
        move_up1 = 0;
    }
    else if(kbd_scan == S_BREAK)
    {
        move_down1 = 0;
    }
    if(move_up2){
        vg_draw_rectangle(racket2.x, racket2.y, racket_width, racket_height, 0);
        racket2.y -= racket2.speed;
        flag = false;   
    }
    
    else if(kbd_scan == UP_ARROW){
        move_up2 = 1;
        move_down2 = 0;
        vg_draw_rectangle(racket2.x, racket2.y, racket_width, racket_height, 0);
        racket2.y -= racket2.speed;
        flag = false;  
    }
    
    if(move_down2){
        vg_draw_rectangle(racket2.x, racket2.y, racket_width, racket_height, 0);
        racket2.y += racket2.speed;
        flag = false;         
    }
    
    else if(kbd_scan == DOWN_ARROW){
        move_down2 = 1;
        move_up2 = 0;
        vg_draw_rectangle(racket2.x, racket2.y, racket_width, racket_height, 0);
        racket2.y += racket2.speed;
        flag = false;  
    }
    if (racket2.y > 688) //limite em baixo (768-15)
        racket2.y = 688;
    if (racket2.y < 1) //limite em cima(contando com o tamanho da raquete)
        racket2.y = 1;
    xpm_draw(racket2.xpm, racket2.x , racket2.y); //desenha raquete da direita
    
    
    if(move_up1){
        vg_draw_rectangle(racket1.x, racket1.y, racket_width, racket_height, 0);
        racket1.y -= racket1.speed;
        flag = false;       
    }
    
    else if(kbd_scan == W_MAKE){
        move_up1 = 1;
        move_down1 = 0;
        vg_draw_rectangle(racket1.x, racket1.y, racket_width, racket_height, 0);
        racket1.y -= racket1.speed;
        flag = false;  
    }
    if(move_down1){
        vg_draw_rectangle(racket1.x, racket1.y, racket_width, racket_height, 0);
        racket1.y += racket1.speed;
        flag = false;         
    }
    else if(kbd_scan == S_MAKE){
        move_down1 = 1;
        move_up1 = 0;
        vg_draw_rectangle(racket1.x, racket1.y, racket_width, racket_height, 0);
        racket1.y += racket1.speed;
        flag = false;  
    }
    if (racket1.y > 688) //limite em baixo (768-80)
        racket1.y = 688;
    if (racket1.y < 1) //limite em cima
        racket1.y = 1;
    xpm_draw(racket1.xpm, racket1.x , racket1.y); // desenha raquete da esquerda

}


int ball_movement()
{
    if (time_of_day == DAY)
    {
        ball.xpm = (xpm_row_t**)&ball_day;
    }
    else
    {
        ball.xpm = (xpm_row_t**)&ball_night;

    }

    int ball_width = 15;
    int ball_height = 15;

    vg_draw_rectangle(ball.x, ball.y, ball_width, ball_height, 0);

    if (ball.x >= 490 & ball.x <= 520) //bola esta sobre as linhas do meio campo
        xpm_draw((xpm_row_t*)mid_line.xpm, mid_line.x, mid_line.y);
    if (ball.x >= 335 && ball.x <= 420 && ball.y >= 75 && ball.y <= 210) //bola esta sobre a pontuacao do jogador da esquerda
        xpm_draw((xpm_row_t*)points_player1.xpm, points_player1.x, points_player1.y);
    if (ball.x >= 585 && ball.x <= 670 && ball.y >= 75 && ball.y <= 210) //bola esta sobre a pontuacao do jogador da direita
        xpm_draw((xpm_row_t*)points_player2.xpm, points_player2.x, points_player2.y);    
        
    ball.x += ball.x_speed;
    ball.y += ball.y_speed;

    if (ball.x > 970){  //limite a direita (985 - (15))
    // parte cima da bola maior que parte de baixo da raquete
    // parte de baixo da bola maior que a parte de cima da raquete(e ponto)
        if((ball.y > racket2.y + 80) || (ball.y + 15 < racket2.y)){
            // nao colidiu
            if(ball.x > 1009){
                return 1;
            }
        }
        else {  //bateu na raquete direita
            if(ball.x_speed > 0)
                ball.x_speed++;
            else
                ball.x_speed--;

            ball.x = 970;   //falta tamanho da bola
            ball.x_speed = -ball.x_speed;
            ball.x += ball.x_speed;
            //movimento da raquete altera trajetoria da bola
            if(move_up2)
                ball.y_speed -= 3;
            else if(move_down2)
                ball.y_speed += 3;
        }
    }

    else if (ball.x < 30){  // limite a esquerda (15 + 15)
        if((ball.y > racket1.y + 80) || (ball.y + 15 < racket1.y)){
            if(ball.x < 0){
                return 2;
            }
        }
    
        else{   //bateu na raquete esquerda
            if(ball.x_speed > 0)
                ball.x_speed++;
            else
                ball.x_speed--;
            ball.x = 30;
            ball.x_speed = -ball.x_speed;   
            ball.x += ball.x_speed;
            //movimento da raquete altera trajetoria da bola
            if(move_up1)
                ball.y_speed -= 3;
            else if(move_down1)
                ball.y_speed += 3;
        }
    }
    if (ball.y > 738){  //limite em baixo (768-30)
        ball.y = 738;   // falta tamanho da bola
        ball.y_speed = -ball.y_speed;
        ball.y += ball.y_speed;
    }

    else if (ball.y < 0){  //limite em cima
        ball.y = 0;
        ball.y_speed = -ball.y_speed;
        ball.y += ball.y_speed;
    }
    xpm_draw(ball.xpm, ball.x, ball.y);

    return 0;

}




