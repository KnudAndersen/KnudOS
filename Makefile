TARGET_32:=i686-elf
TARGET_64:=x86_64-elf

CROSS_DIR:=./cross
LIB_32=./boot
LIB_64=./src
BUILD:=./build

CC_32:=$(shell find $(CROSS_DIR) -name '$(TARGET_32)-gcc')
CC_64:=$(shell find $(CROSS_DIR) -name '$(TARGET_64)-gcc')
ifeq ($(strip $(CC_32)),)
$(error 32-bit cross compiler not found for $(TARGET_32))
endif
ifeq ($(strip $(CC_64)),)
$(error 64-bit cross compiler not found for $(TARGET_64))
endif

CFLAGS_32:=-Wall -Wextra -ffreestanding -fno-pic -fno-pie -masm=intel -gdwarf-4
CFLAGS_64:=-mcmodel=large -Wall -Wextra -ffreestanding -fno-pic -fno-pie -masm=intel -gdwarf-4

LINK_32:=$(CC_32)
LINK_64:=$(CC_64)
LFLAGS_32:=-ffreestanding -nostdlib -static -lgcc
LFLAGS_64:=-ffreestanding -nostdlib -static -lgcc
LINK_SCRIPT_32=$(LIB_32)/link_32.ld
LINK_SCRIPT_64=$(LIB_64)/klink.ld

ASM_32:=nasm
ASM_64:=$(ASM_32)
AFLAGS_32:=-felf32 -F dwarf -g
AFLAGS_64:=-felf64 -F dwarf -g

SRC_32:=$(shell find $(LIB_32) -name '*.c' -or -name '*.asm')
OBJ_32:=$(SRC_32:%=$(BUILD)/%.32.o)

SRC_64:=$(shell find $(LIB_64) -name '*.c' -or -name '*.asm')
OBJ_64:=$(SRC_64:%=$(BUILD)/%.64.o)

LOADER_BIN:=$(BUILD)/loader.BIN
KERNEL_BIN:=$(BUILD)/kernel.BIN

ISO:=$(BUILD)/knud_os.iso
ISO_DIR:=$(BUILD)/iso
GRUB_CFG:=$(ISO_DIR)/boot/grub/grub.cfg

.PHONY: all iso bin clean qemu

all: bin iso
bin: $(LOADER_BIN) $(KERNEL_BIN)
iso: bin $(ISO)

$(LOADER_BIN): $(OBJ_32) $(LINK_SCRIPT_32) 
	$(LINK_32) $(LFLAGS_32) -T $(LINK_SCRIPT_32) $(OBJ_32) -o $@ -lgcc
$(KERNEL_BIN): $(OBJ_64) $(LINK_SCRIPT_64)
	$(LINK_64) $(LFLAGS_64) -T $(LINK_SCRIPT_64) $(OBJ_64) -o $@ -lgcc

$(BUILD)/%.c.32.o: %.c
	mkdir -p $(dir $@)
	$(CC_32) $(CFLAGS_32) -c $< -o $@

$(BUILD)/%.asm.32.o: %.asm
	mkdir -p $(dir $@)
	$(ASM_32) $(AFLAGS_32) $< -o $@

$(BUILD)/%.c.64.o: %.c
	mkdir -p $(dir $@)
	$(CC_64) $(CFLAGS_64) -c $< -o $@

$(BUILD)/%.asm.64.o: %.asm
	mkdir -p $(dir $@)
	mkdir -p $(ISO_DIR)/boot/grub
	$(ASM_64) $(AFLAGS_64) $< -o $@

clean:
	rm -rf $(BUILD)

$(ISO): $(GRUB_CFG) $(LOADER_BIN) $(KERNEL_BIN)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(LOADER_BIN) $(ISO_DIR)/boot
	cp $(KERNEL_BIN) $(ISO_DIR)/boot
	/usr/bin/grub-mkrescue $(ISO_DIR) -o $@	

$(GRUB_CFG): $(LOADER_BIN) $(KERNEL_BIN)
	mkdir -p $(ISO_DIR)/boot/grub
	echo -e "menuentry \"KnudOS\" {" > $(GRUB_CFG)
	echo -e "\tmultiboot /boot/$(notdir $(LOADER_BIN))" >> $(GRUB_CFG)
	echo -e "\tmodule /boot/$(notdir $(KERNEL_BIN)) KnudKernel" >> $(GRUB_CFG)
	echo -e "}" >> $(GRUB_CFG)

qemu: $(ISO)
	qemu-system-x86_64 -m 8G -cdrom $(ISO) -gdb tcp::26000 -S

qemu32: $(ISO)
	qemu-system-i386 -m 8G -cdrom $(ISO) -gdb tcp::26000 -S

