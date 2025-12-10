#include "drivers/pl011.h"
#include "printf.h"
#include "interrupt.h"
#include "drivers/timer.h"
#include "os_core/process.h"

void putc(void *p, char c){
    if(c == '\n'){
        pl011_sendc('\r');
    }
    pl011_sendc(c);
}
void task1(uint64_t arg);
void task2(uint64_t arg);
void task3(uint64_t arg);

void kernel_main(void)
{
    pl011_init();
    init_printf(0, putc);
    printf("Booting kernel...\n");
    
    // Initialize memory and scheduler
    sched_init();
    
    // Create test tasks
    fork((uint64_t)task1, 1);
    fork((uint64_t)task2, 2);
    fork((uint64_t)task3, 3);
    
    // Enable timer
    timer_init();
    irq_vector_init();
    enable_interrupt_controller();
    enable_irq();
    
    // Idle loop
    while (1) {
        schedule();
    }
}

// Test tasks
void task1(uint64_t arg)
{
    while (1) {
        printf("Task %lu: tick=%lu, Q%d\n", 
               arg, current->ticks, current->queue_level);
        for (volatile int i = 0; i < 1000000; i++);  // Busy work
    }
}

void task2(uint64_t arg) { 
    while (1) {
        printf("Task %lu: tick=%lu, Q%d\n", 
               arg, current->ticks, current->queue_level);
        for (volatile int i = 0; i < 1000000; i++);  // Busy work
    }
}
void task3(uint64_t arg) { 
    while (1) {
        printf("Task %lu: tick=%lu, Q%d\n", 
               arg, current->ticks, current->queue_level);
        for (volatile int i = 0; i < 1000000; i++);  // Busy work
    }
}