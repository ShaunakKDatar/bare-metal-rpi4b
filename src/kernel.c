#include "pl011.h"
#include "printf.h"
#include "interrupt.h"
#include "timer.h"

void putc(void *p, char c){
    if(c == '\n'){
        pl011_sendc('\r');
    }
    pl011_sendc(c);
}

void kernel_main(void){
    pl011_init();
    init_printf(0, putc);
    irq_vector_init();
    timer_init();
    enable_interrupt_controller();
    enable_irq();
    
    printf("\nWaiting for interrupts...\n");
    while(1);
}