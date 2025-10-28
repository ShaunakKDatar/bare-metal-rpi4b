#pragma once
#include "base.h"

#define TIMER_BASE          (RPI_PERIPHERAL_BASE + 0x00003000)
#define TIMER_CS            (TIMER_BASE + 0x00)
#define TIMER_CLO           (TIMER_BASE + 0x04)
#define TIMER_CHI           (TIMER_BASE + 0x08)
#define TIMER_C0            (TIMER_BASE + 0x0C)
#define TIMER_C1            (TIMER_BASE + 0x10)
#define TIMER_C2            (TIMER_BASE + 0x14)
#define TIMER_C3            (TIMER_BASE + 0x18)

#define TIMER_CS_M0         (1 << 0)
#define TIMER_CS_M1         (1 << 1)
#define TIMER_CS_M2         (1 << 2)
#define TIMER_CS_M3         (1 << 3)

#define SYSTEM_TIMER_IRQ_0  (1 << 0)
#define SYSTEM_TIMER_IRQ_1  (1 << 1)
#define SYSTEM_TIMER_IRQ_2  (1 << 2)
#define SYSTEM_TIMER_IRQ_3  (1 << 3)

void timer_init(void);
void handle_timer_irq(void);