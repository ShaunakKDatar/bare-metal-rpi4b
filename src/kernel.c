// #include "mini_uart.h"

// void kernel_main(){
//     uart_init();
//     uart_send_string("Welcome to the raspberry pi baremetal OS");

//     while(1){
//         uart_send(uart_recv());
//     }
// }
void kernel_main() {
    // Direct register access avoiding your abstraction layer
    // Using known QEMU-compatible addresses
    volatile unsigned int* AUX_ENABLES = (volatile unsigned int*)0x3F215004;
    volatile unsigned int* AUX_MU_IO = (volatile unsigned int*)0x3F215040;
    
    // Enable mini UART
    *AUX_ENABLES = 1;
    
    // Send characters directly without checking status
    *AUX_MU_IO = 'A';
    *AUX_MU_IO = 'B';
    *AUX_MU_IO = 'C';
    *AUX_MU_IO = '\r';
    *AUX_MU_IO = '\n';
    
    // Loop forever
    while(1);
}