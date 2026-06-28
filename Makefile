UNAME_M := $(shell uname -m)

ifeq ($(UNAME_M),x86_64)
    ARCH ?= x86
else ifeq ($(UNAME_M),amd64)
    ARCH ?= x86
else ifeq ($(UNAME_M),i386)
    ARCH ?= x86
else ifeq ($(UNAME_M),aarch64)
    ARCH ?= arm64
else ifeq ($(UNAME_M),arm64)
    ARCH ?= arm64
else
    ARCH ?= x86
endif

BUILD_DIR = build
SRC_DIR = src

ifeq ($(ARCH),x86)
    CC = gcc
    AS = nasm
    LD = ld
    LDFLAGS = -m elf_i386 -T $(SRC_DIR)/arch/x86/boot/link.ld
    ASFLAGS = -f elf32
    CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -Iinclude -Isrc/arch/x86/include -c
    QEMU = qemu-system-i386 -kernel
else ifeq ($(ARCH),arm64)
    CC = aarch64-linux-gnu-gcc
    AS = aarch64-linux-gnu-as
    LD = aarch64-linux-gnu-ld
    LDFLAGS = -T $(SRC_DIR)/arch/arm64/boot/link.ld
    ASFLAGS = 
    CFLAGS = -ffreestanding -nostdlib -fno-stack-protector -fno-pie -Iinclude -Isrc/arch/arm64/include -c
    QEMU = qemu-system-aarch64 -machine virt -cpu cortex-a57 -kernel
endif

C_SOURCES = $(shell find $(SRC_DIR) -name "*.c" ! -path "$(SRC_DIR)/arch/*" -o -path "$(SRC_DIR)/arch/$(ARCH)/*.c")
ASM_SOURCES = $(shell find $(SRC_DIR) -name "*.asm" ! -path "$(SRC_DIR)/arch/*" -o -path "$(SRC_DIR)/arch/$(ARCH)/*.asm")

OBJ = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%.o, $(C_SOURCES) $(ASM_SOURCES))

all: $(BUILD_DIR)/kernel

$(BUILD_DIR)/kernel: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.asm.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run: $(BUILD_DIR)/kernel
	$(QEMU) $<