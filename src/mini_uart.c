#include "mini_uart.h"
#include "gpio.h"


void uart_init(){

    gpio_select(14,ALT_FN_0);
    gpio_select(15,ALT_FN_0);
    gpio_enable(14);
    gpio_enable(15);

    put32(AUX_ENABLES,1);
    AUX_MU_BASE->CNTL = 0;
    AUX_MU_BASE->IER = 0;
    AUX_MU_BASE->LCR = 3;
    AUX_MU_BASE->MCR = 0;
    AUX_MU_BASE->BAUD = 541;
    AUX_MU_BASE->CNTL = 3;

    uart_send('\r');
    uart_send('\n');
    uart_send('\n');
}
char uart_recv(){
    while(!(AUX_MU_BASE->LSR & 1));

    return AUX_MU_BASE->IO & 0xFF;
}

void uart_send(char c){
    while(!(AUX_MU_BASE->LSR & 0x20));

    AUX_MU_BASE->IO = c;
}

void uart_send_string(char* str){
    while(*str){
        if (*str == '\n') {
            uart_send('\r');
        }

        uart_send(*str);
        str++;
    }
}

