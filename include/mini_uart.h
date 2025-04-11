#pragma once
#include "base.h"
#include "utils.h"

#define AUX_ENABLES   (*(volatile uint32_t *)(RPI_PERIPHERAL_BASE + 0x00215004))

typedef struct {
    volatile uint32_t IO;         // Offset 0x00
    volatile uint32_t IER;        // Offset 0x04
    volatile uint32_t IIR;        // Offset 0x08
    volatile uint32_t LCR;        // Offset 0x0C
    volatile uint32_t MCR;        // Offset 0x10
    volatile uint32_t LSR;        // Offset 0x14
    volatile uint32_t MSR;        // Offset 0x18
    volatile uint32_t SCRATCH;    // Offset 0x1C
    volatile uint32_t CNTL;       // Offset 0x20
    volatile uint32_t STAT;       // Offset 0x24
    volatile uint32_t BAUD;       // Offset 0x28
} aux_mu_regs_t;

#define AUX_MU_BASE   ((volatile aux_mu_regs_t *)(RPI_PERIPHERAL_BASE + 0x00215040))




void uart_init();
char uart_recv();
void uart_send(char c);
void uart_send_string(char* str);

