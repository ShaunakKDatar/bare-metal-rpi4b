#include "mm.h"
#define DEBUG_UART
    .section .text.boot
    .global _start

_start:
    /* arrive here as core-0 only */
    mrs x0, mpidr_el1
    and x0, x0, #0xFF
    cbnz x0, proc_hang          // other cores park

master:
    /* clear .bss */
    adr x0, bss_begin
    adr x1, bss_end
    sub x1, x1, x0
    bl  memzero

    /* set up a temporary stack */
    ldr x0, =0x400000
    mov sp, x0

    bl  kernel_main

proc_hang:
    wfe
    b   proc_hang
