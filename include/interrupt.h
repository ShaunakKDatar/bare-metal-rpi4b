#pragma once
#include "base.h"

#define IRQ_BASE            (RPI_PERIPHERAL_BASE + 0x0000B200)

#define IRQ_PENDING_0       (IRQ_BASE + 0x00)
#define IRQ_PENDING_1       (IRQ_BASE + 0x04)
#define IRQ_PENDING_2       (IRQ_BASE + 0x08)

#define IRQ_ENABLE_0        (IRQ_BASE + 0x10)
#define IRQ_ENABLE_1        (IRQ_BASE + 0x14)
#define IRQ_ENABLE_2        (IRQ_BASE + 0x18)

#define IRQ_DISABLE_0       (IRQ_BASE + 0x1C)
#define IRQ_DISABLE_1       (IRQ_BASE + 0x20)
#define IRQ_DISABLE_2       (IRQ_BASE + 0x24)

#define SYSTEM_TIMER_IRQ_0  (1 << 0)
#define SYSTEM_TIMER_IRQ_1  (1 << 1)
#define SYSTEM_TIMER_IRQ_2  (1 << 2)
#define SYSTEM_TIMER_IRQ_3  (1 << 3)

#ifndef __ASSEMBLER__
void irq_vector_init(void);
void enable_irq(void);
void disable_irq(void);
void ret_from_fork(void);
#endif
void enable_interrupt_controller(void);
void handle_irq(void);
void handle_sync(void);
