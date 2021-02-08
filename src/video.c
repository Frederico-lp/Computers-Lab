#include <lcom/lcf.h>
#include "i8042.h"
#include "video.h"
#include "video_macros.h"
#include "keyboard.h"
#include "timer.h"
#include "rtc.h"

extern uint8_t kbd_scan, kbd_temp;
extern unsigned int counter;

typedef enum {DAY, NIGHT} Time_Day;

Time_Day time_of_day;

static char *video_mem;		/* Process (virtual) address to which VRAM is mapped */
static char *scd_buffer;

static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned bytes_per_pixel; 
static unsigned int buffer_size;

void *(vg_init)(uint16_t mode)
{
    vbe_mode_info_t info;
    if(vbe_get_mode_info(mode, &info)){
        printf("vbe_get_mode_info failed in vg_init\n");
        return NULL;
    }
    struct minix_mem_range mr;
    unsigned int vram_base;  /* VRAM's physical addresss */
    unsigned int vram_size;  /* VRAM's size, but you can use the frame-buffer size, instead */
    int r;				    

    /* Use VBE function 0x01 to initialize vram_base and vram_size */
    //horizontal and vertical bits
    h_res = info.XResolution;  
    v_res = info.YResolution;
    bits_per_pixel = info.BitsPerPixel;
    bytes_per_pixel = (bits_per_pixel + 7) / 8;
    vram_base = info.PhysBasePtr;
    vram_size = h_res * v_res * bytes_per_pixel;
    buffer_size = vram_size;

    /* Allow memory mapping */
    // base d outro seria limite do primeiro
    mr.mr_base = (phys_bytes) vram_base;	
    mr.mr_limit = mr.mr_base + vram_size;  

    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    /* Map memory */

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    scd_buffer = malloc(vram_size);

    if(video_mem == MAP_FAILED)
    panic("couldn't map video memory");

    /// o q falta aqui?
    struct reg86 r86;
   
    /* Specify the appropriate register values */

    memset(&r86, 0, sizeof(r86));	/* zero the structure */

    r86.ax = SET_VBE_MODE;
    r86.bx = mode | BIT(14);   
    r86.intno = 0x10; /* BIOS video services */


    /* Make the BIOS call */

    if( sys_int86(&r86) != OK ) {
        printf("\tvg_exit(): sys_int86() failed \n");
        return NULL;
    }
    return video_mem;

}

void buffer_cpy(){
    memcpy(video_mem, scd_buffer, buffer_size);
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color)
{
    unsigned int i;
    if (color == 20)
        return 1;
    for(i = 0;i < bytes_per_pixel;i++){
            *(scd_buffer + i + bytes_per_pixel * (h_res * y + x) ) = color >> (8 * i);
    }
    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len,uint32_t color)
{
    int i;
    for(i = 0;i < len;i++){
        vg_draw_pixel(x, y, color); //falta coisas, x e y?
        x++;
    }

    return 0;
}


int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    ////////////////////////////
    int hour;
    hour = rtc_read_hour();
    if(hour < 8 || hour > 19){
        time_of_day = NIGHT;
    }
    else {
        time_of_day = DAY;
    }
    ////////////////////////////
    if (time_of_day == DAY)
        color = 63;
    else
        color = 0;
    
    for(int i = 0;i < height;i++){
        vg_draw_hline(x, y + i, width, color);


    }
    return 0;
}


int run_until_esc(){
    uint8_t bit_no;
    if (keyboard_subscribe_int(&bit_no) != 0){    // bit_no e a linha
        printf("keyboard_susbcribe_int failed");
        return 1;
    }
    int ipc_status, r;
    message msg;
    while(kbd_scan != ESC) {  
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
        }
        if (is_ipc_notify(ipc_status)) { 
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: 
                if (msg.m_notify.interrupts & BIT(bit_no)) { 
                    kbc_ih();
                
                }
                break;
            default:
            break; 
        }
        } else { 
        }
    }
    if(keyboard_unsubscribe_int() != 0){
        printf("keyboard_unsubscribe_int failed");
        return 1;
    }
    return 0;
}

void xpm_draw(xpm_map_t xpm, uint16_t x, uint16_t y){
    uint8_t *map;
    xpm_image_t img;
    map = xpm_load(xpm, XPM_INDEXED, &img);
    int i, j;
    for(i = 0;i < img.height;i++){
        for(j = 0;j < img.width;j++){
            vg_draw_pixel(x + j, y + i, map[(i * img.width + j ) * bytes_per_pixel] );
            //i * img.width linha; j coluna; 
        }
    }
}

