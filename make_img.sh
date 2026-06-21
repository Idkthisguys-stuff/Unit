#!/bin/bash
set -e

# 1. Create a blank 64MB raw image file
dd if=/dev/zero of=unit.img bs=1M count=64

# 2. Format it directly as a FAT32 file system
mformat -F -i unit.img ::

# 3. Recreate the exact directory structure inside the image
mmd -i unit.img ::/EFI
mmd -i unit.img ::/EFI/BOOT
mmd -i unit.img ::/boot
mmd -i unit.img ::/boot/limine

# 4. Copy the bootloader, config, and kernel into the image
mcopy -i unit.img iso_root/EFI/BOOT/BOOTX64.EFI ::/EFI/BOOT/BOOTX64.EFI
mcopy -i unit.img iso_root/boot/limine/limine.cfg ::/boot/limine/limine.cfg
mcopy -i unit.img iso_root/unit.elf ::/unit.elf

echo "Real FAT32 disk image 'unit.img' created successfully!"