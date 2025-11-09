#include "drivers/timer.h"
#include "printf.h"
#include "os_core/process.h"

const unsigned int interval = 200000;  // 0.2 seconds (1MHz clock)
unsigned int curVal = 0;

void timer_init(void)
{
    curVal = BCM2711_REG(TIMER_CLO);
    curVal += interval;
    BCM2711_REG(TIMER_C1) = curVal;
}

void handle_timer_irq(void)
{
    curVal += interval;
    BCM2711_REG(TIMER_C1) = curVal;
    BCM2711_REG(TIMER_CS) = TIMER_CS_M1;
    timer_tick();
    printf("Timer interrupt received\n");
}