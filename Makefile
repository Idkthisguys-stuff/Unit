# Define variables
CC = gcc
AS = nasm
BUILD_DIR = build
SRC_DIR = src

KERNEL = $(BUILD_DIR)/kernel

all: $(KERNEL)

$(KERNEL): $(BUILD_DIR)/kasm.o $(BUILD_DIR)/kc.o
	ld -m elf_i386 -o $(KERNEL) -T $(SRC_DIR)/link.ld $^

$(BUILD_DIR)/kasm.o: $(SRC_DIR)/kernel.asm
	mkdir -p $(BUILD_DIR)
	$(AS) -f elf32 $< -o $@

$(BUILD_DIR)/kc.o: $(SRC_DIR)/kernel.c
	mkdir -p $(BUILD_DIR)
	$(CC) -m32 -ffreestanding -fno-pie -fno-stack-protector -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run:
	qemu-system-i386 -kernel $(BUILD_DIR)/kernel