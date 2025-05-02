#include "mini_uart.h"

void kernel_main(){
    uart_init();
    uart_send_string("Hello World: Shaunak Datar\n");
    uart_send_string("mini-uart configured. Letss see where this goes :)\n");

    while(1){
        uart_send(uart_recv());
    }
}
