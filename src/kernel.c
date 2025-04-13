#include "mini_uart.h"

void kernel_main(){
    uart_init();
    uart_send_string("Welcome to the raspberry pi baremetal OS");

    while(1){
        uart_send(uart_recv());
    }
}
