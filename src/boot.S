#include "mm.h"
#include "sysregs.h"
#define DEBUG_UART
    .section .text.boot
    .global _start

_start:
    # arrive here as core-0 only
    mrs x0, mpidr_el1
    and x0, x0, #0xFF
    cbnz x0, proc_hang

master:
    ldr x0, =SCTLR_VALUE_MMU_DISABLED
    msr sctlr_el1, x0

    ldr x0, =HCR_VALUE
    msr hcr_el2, x0

    ldr x0, =SCR_VALUE
    msr scr_el3, x0

    ldr x0, =SPSR_VALUE
    msr spsr_el3, x0

    adr x0, el1_entry
    msr elr_el3, x0
    eret

el1_entry:
    msr spsel, #1
    # clear .bss
    adr x0, bss_begin
    adr x1, bss_end
    sub x1, x1, x0
    bl  memzero

    # set up a temporary stack
    ldr x0, =0x400000
    mov sp, x0

    bl  kernel_main

proc_hang:
    wfe
    b   proc_hang