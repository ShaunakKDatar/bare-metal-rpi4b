# --------------------------------------------------------------
#  Raspberry Pi 4 bare-metal build
# --------------------------------------------------------------

# --- tool-chain prefix (override on command line if needed) ---
#   examples:
#     brew install aarch64-elf-gcc     → prefix = aarch64-elf
#     sudo apt install gcc-aarch64-linux-gnu → prefix = aarch64-linux-gnu
TOOLCHAIN ?= aarch64-none-elf

CC      := $(TOOLCHAIN)-gcc
LD      := $(TOOLCHAIN)-ld
OBJCOPY := $(TOOLCHAIN)-objcopy

# --- optional debug banner ------------------------------------
ifeq ($(DEBUG),1)
    $(info ▶ DEBUG_UART enabled)
    DEBUGFLAG := -DDEBUG_UART
else
    DEBUGFLAG :=
endif

# --- flags -----------------------------------------------------
INCDIR  := -Iinclude
CFLAGS  := -Wall -Wextra -ffreestanding -nostdlib -nostartfiles \
           -mgeneral-regs-only -g $(INCDIR) $(DEBUGFLAG)
ASFLAGS := -nostdlib -x assembler-with-cpp $(INCDIR) $(DEBUGFLAG)
LDFLAGS := -T src/linker.ld

# --- file lists ------------------------------------------------
C_SRC   := $(wildcard src/*.c)
S_SRC   := $(wildcard src/*.S)
OBJDIR  := build
C_OBJS  := $(patsubst src/%.c,$(OBJDIR)/%.o,$(C_SRC))
S_OBJS  := $(patsubst src/%.S,$(OBJDIR)/%.o,$(S_SRC))
OBJS    := $(C_OBJS) $(S_OBJS)
DEPS    := $(OBJS:.o=.d)

# --------------------------------------------------------------
#  Rules
# --------------------------------------------------------------
.PHONY: all clean
all: kernel8.img

# object files
$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

$(OBJDIR)/%.o: src/%.S | $(OBJDIR)
	$(CC) $(ASFLAGS) -MMD -c $< -o $@

# final link & image
kernel8.elf: $(OBJS) src/linker.ld
	$(LD) $(LDFLAGS) $(OBJS) -o $(OBJDIR)/kernel8.elf
	cp $(OBJDIR)/kernel8.elf $@

kernel8.img: kernel8.elf
	$(OBJCOPY) -O binary $(OBJDIR)/kernel8.elf $@

# helper dirs & clean
$(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR) kernel8.elf kernel8.img

# include auto-generated deps
-include $(DEPS)
