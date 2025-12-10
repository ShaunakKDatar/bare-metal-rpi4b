# The Bare-metal Raspberry Pi Operating System
This project aims to create a bare-metal operating system for the raspberry pi 4b. 
The Raspberry Pi is based on the AARCH64 architecture
Till now this project has added support for:
- Booting into the kernel image
- Switching exception level
- Pl011 and Mini-uart support
- Vector table and timer interrupts
- Multilevel Feedback queue Scheduler

Lets look at each section indivdually:

## Booting into the kernel image
For booting the Raspberry Pi internal boot handles a lot of things for us, The boot process is very intering in the RPI:
- The Videocore Boots first, initializes the clock, power and other basic stuff( This explains the rainbow we see at boot)
- Its after initialisation of the GPU that the CPU boots
- The kernel image is read from the SD-Card, the device tree configurations are set according to the config.txt
- This is where we come in. We create the kernel image based on the recepies in the Makefile.

boot.S is the entry point of this project:
- Only one core is activated. other 3 cores(accessed by the mpdir_el1 register) are put in an eternal state of hang
- The bss section size is cleared
- The stack pointer is initialised to the LOW_MEMORY
The memory structure of our project is:
- 0x00000000 to 0x00400000 is reserved as LOW_MEMORY. This is the place we store the kernel code, data, and boot stack.
- We have 4kB pages.
- 512 entries per page table
- 0x00400000 - 0xFE000000 is the memory available for the pages
- 0xFE00000 onwards RPI reserved for the peripherals details given here: [Datasheet](https://datasheets.raspberrypi.com/bcm2711/bcm2711-peripherals.pdf)

After the Stack is initialise we make the jump to the kernel_main function which stores our application code.

## ARM Exception Levels (EL0–EL3)

ARMv8 processors (like the one in the Raspberry Pi 4) define **four Exception Levels**, which represent different privilege modes of execution. Each Exception Level controls what instructions are allowed, what memory can be accessed, and what registers can be modified.

**Lower numbers mean less privilege. Higher numbers mean more control over the system.**

---

### **EL0 – User Programs (Least Privileged)**

**EL0 is where normal applications run.**  
At this level, a program can only use:

- General-purpose registers (X0–X30)
- A stack pointer (SP)
- Basic load/store and arithmetic instructions

What EL0 *cannot* do:

- Modify virtual memory mappings
- Touch system control registers
- Access other program’s memory

Because of these restrictions, operating systems run user processes at EL0 to provide **process isolation**. Each process gets its own virtual memory layout, and it cannot break into the memory of other processes or the OS.

**→ EL0 = Safe mode for user applications**

---

### **EL1 – Operating System Kernel**

**The OS kernel runs at EL1.**

EL1 has the privileges needed to:

- Configure virtual memory and page tables
- Control system registers
- Manage hardware exceptions and interrupts

An OS uses EL1 to create separate address spaces for processes and then drops to EL0 to run them. When a process needs a service (via a syscall, for example), it traps back into EL1.

**→ EL1 = Where the kernel lives**

---

### **EL2 – Hypervisor Level**

EL2 exists to support **virtualization**, meaning multiple operating systems running on the same hardware.

Use case:

- A **host OS (hypervisor)** runs at EL2
- **Guest OSes** (virtual machines) run at EL1

The hypervisor controls guests the same way an OS controls user processes, ensuring complete separation between virtual machines.

**→ EL2 = Virtualization / Hypervisor mode**

---

### **EL3 – Secure Monitor (TrustZone)**

EL3 is the most privileged level and is used for secure-world operations through **ARM TrustZone**.

Conceptually, the processor is split into:

- **Secure World** (running at EL3 and secure EL1/EL0)
- **Normal World** (regular operating systems)

Code running in the secure world cannot be accessed or modified by the normal world—even if the normal world is compromised. This supports things like:

- Cryptographic key storage
- Secure boot
- Trusted firmware

**→ EL3 = Secure monitor, handles transitions between Secure and Normal worlds**

---

### **Summary Table**

| Exception Level | Who Runs Here? | Purpose |
|----------------|---------------|---------|
| **EL0** | User applications | Safe execution with limited privileges |
| **EL1** | OS kernel | Manages memory, devices, and user processes |
| **EL2** | Hypervisor | Runs virtual machines (multiple OSes) |
| **EL3** | Secure monitor | TrustZone security and “Secure World” control |

---

In our bare-metal Raspberry Pi OS, we configure the system to run **the kernel at EL1** and later will switch to **EL0** for user programs. EL2 and EL3 exist but are not needed unless we build virtualization or secure firmware features.
By default the Raspberry Pi starts at EL3, but because of the initial software running we see EL2 boot.
Hence we create our custom armstub which bypasses this EL3->2 Switching and directlty go from EL3->1
<img width="1392" height="242" alt="EL3" src="https://github.com/user-attachments/assets/61d017a8-22b4-4d27-a54f-76a0e42731f0" />
<img width="1392" height="242" alt="EL1" src="https://github.com/user-attachments/assets/152d2496-5863-47dc-bb9e-0d80a243a0b4" />

## PL011 & Mini-UART in Our OS

The Raspberry Pi 4B exposes two serial interfaces through the BCM2711 SoC: **PL011 UART** and **Mini-UART**. Both implement the same UART protocol (asynchronous serial communication), but they differ in capabilities.

- **PL011 UART** 
- **Mini-UART**

Despite their differences, from a software perspective they both speak the standard UART protocol. UART transfers data without a shared clock, so both devices must agree on:

- **Baud rate**
- **Number of data bits (commonly 8)**
- **Parity (usually none)**
- **Stop bits (usually 1)**

Every byte sent on the TX line is wrapped into a frame and transmitted **LSB first**:
Start Bit | Data Bits | Optional Parity | Stop Bit(s)
The receiver watches the RX line, detects the start bit, samples the incoming bits, and reconstructs the byte using the same parameters. If both devices match settings, communication works reliably with only two wires.
As a demo we displayed the Hello World. **We now have a window to look at our OS.** 
![UART](https://github.com/user-attachments/assets/3436b5d7-50d9-41b0-b1cf-57335ef4f4ef)

## Timer Interrupts
The System Timer is configured to generate an interrupt every 0.2 seconds. The interrupt handler prints the statement once the interrupt is received
<img width="1142" height="660" alt="Timer interrupt" src="https://github.com/user-attachments/assets/c08d5020-1c1b-4b51-acad-d45985f198a2" />
