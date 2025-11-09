// Macro to save all general purpose registers
.macro store_gprs
    sub sp, sp, #256
    stp x0, x1, [sp, #16 * 0]
    stp x2, x3, [sp, #16 * 1]
    stp x4, x5, [sp, #16 * 2]
    stp x6, x7, [sp, #16 * 3]
    stp x8, x9, [sp, #16 * 4]
    stp x10, x11, [sp, #16 * 5]
    stp x12, x13, [sp, #16 * 6]
    stp x14, x15, [sp, #16 * 7]
    stp x16, x17, [sp, #16 * 8]
    stp x18, x19, [sp, #16 * 9]
    stp x20, x21, [sp, #16 * 10]
    stp x22, x23, [sp, #16 * 11]
    stp x24, x25, [sp, #16 * 12]
    stp x26, x27, [sp, #16 * 13]
    stp x28, x29, [sp, #16 * 14]
    str x30, [sp, #16 * 15]
.endm

// Macro to restore all general purpose registers
.macro load_gprs
    ldp x0, x1, [sp, #16 * 0]
    ldp x2, x3, [sp, #16 * 1]
    ldp x4, x5, [sp, #16 * 2]
    ldp x6, x7, [sp, #16 * 3]
    ldp x8, x9, [sp, #16 * 4]
    ldp x10, x11, [sp, #16 * 5]
    ldp x12, x13, [sp, #16 * 6]
    ldp x14, x15, [sp, #16 * 7]
    ldp x16, x17, [sp, #16 * 8]
    ldp x18, x19, [sp, #16 * 9]
    ldp x20, x21, [sp, #16 * 10]
    ldp x22, x23, [sp, #16 * 11]
    ldp x24, x25, [sp, #16 * 12]
    ldp x26, x27, [sp, #16 * 13]
    ldp x28, x29, [sp, #16 * 14]
    ldr x30, [sp, #16 * 15]
    add sp, sp, #256
.endm

// Exception vector table (must be 2KB aligned)
.globl vectors
.align 11
vectors:
    // Current EL with SP0
    .balign 0x80
    b handle_irq_entry                  // Synchronous
    .balign 0x80
    b handle_irq_entry                  // IRQ
    .balign 0x80
    b handle_irq_entry                  // FIQ
    .balign 0x80
    b handle_irq_entry                  // SError

    // Current EL with SPx
    .balign 0x80
    b handle_irq_entry                  // Synchronous
    .balign 0x80
    b handle_irq_entry          // IRQ - THIS IS THE ONE WE USE
    .balign 0x80
    b handle_irq_entry                  // FIQ
    .balign 0x80
    b handle_irq_entry                  // SError

    // Lower EL using AArch64
    .balign 0x80
    b handle_irq_entry                  // Synchronous
    .balign 0x80
    b handle_irq_entry                  // IRQ
    .balign 0x80
    b handle_irq_entry                  // FIQ
    .balign 0x80
    b handle_irq_entry                  // SError

    // Lower EL using AArch32
    .balign 0x80
    b handle_irq_entry                  // Synchronous
    .balign 0x80
    b handle_irq_entry                  // IRQ
    .balign 0x80
    b handle_irq_entry                  // FIQ
    .balign 0x80
    b handle_irq_entry                  // SError

// IRQ handler entry point
handle_irq_entry:
    store_gprs
    bl handle_irq
    load_gprs
    eret

.global ret_from_fork
ret_from_fork:
    bl schedule_tail
    mov x0, x20 // arg
    blr x19 // jump to the address we gave in fork

// Error handler - infinite loop
.globl err_hang
err_hang:
    wfe
    b err_hang

// Initialize vector table
.globl irq_vector_init
irq_vector_init:
    adr x0, vectors
    msr vbar_el1, x0
    ret

// Enable IRQ interrupts
.globl enable_irq
enable_irq:
    msr daifclr, #2
    isb
    ret

// Disable IRQ interrupts
.globl disable_irq
disable_irq:
    msr daifset, #2
    ret