#include "mini_uart.h"
#include "printf.h"
#include "mm.h"
#include "pl011.h"

void putc(void *p, char c){
    if(c =='\n'){
        pl011_sendc('\r');
    }

    pl011_sendc(c);
}

void kernel_main(){
    pl011_init();
    init_printf(0, putc);
    printf("Hello World: Shaunak Datar\n");
    printf("mini-uart configured. Letss see where this goes :)\n");
    int exception_level = get_el();
    printf("The Current Excetion Level is: %d", exception_level);
    while(1){
        pl011_sendc(pl011_recv());
    }
}
