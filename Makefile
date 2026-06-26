CC = gcc
AS = nasm
BUILD_DIR = build
SRC_DIR = src

C_SOURCES = $(shell find $(SRC_DIR) -name "*.c")
ASM_SOURCES = $(shell find $(SRC_DIR) -name "*.asm")

OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.c.o, $(C_SOURCES))
OBJ += $(patsubst $(SRC_DIR)/%.asm, $(BUILD_DIR)/%.asm.o, $(ASM_SOURCES))

CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -Iinclude -Isrc/arch/x86/include -c

all: $(BUILD_DIR)/kernel

$(BUILD_DIR)/kernel: $(OBJ)
	ld -m elf_i386 -o $@ -T $(SRC_DIR)/arch/x86/boot/link.ld $^

$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.asm.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(AS) -f elf32 $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run: $(BUILD_DIR)/kernel
	qemu-system-i386 -kernel $<