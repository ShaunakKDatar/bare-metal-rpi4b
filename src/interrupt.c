#include "interrupt.h"
#include "drivers/timer.h"
#include "printf.h"

void enable_interrupt_controller(void)
{
    // Enable system timer interrupt 1
    BCM2711_REG(IRQ_ENABLE_0) = SYSTEM_TIMER_IRQ_1;
}

void handle_irq(void)
{
    uint32_t irq = BCM2711_REG(IRQ_PENDING_0);
    
    if (irq & SYSTEM_TIMER_IRQ_1) {
        handle_timer_irq();
    }
}