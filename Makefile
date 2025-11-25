PP_EXTN:=CPP
SRC_32_RAW_EXTNS:=c ld S
SRC_64_RAW_EXTNS:=c ld S

TARGET_32:=i686-elf
TARGET_64:=x86_64-elf
CROSS_DIR:=./.cross

CC_32:=$(shell find $(CROSS_DIR) -name '$(TARGET_32)-gcc')
CC_64:=$(shell find $(CROSS_DIR) -name '$(TARGET_64)-gcc')

ifeq ($(strip $(CC_32)),)
	$(error 32-bit cross compiler not found for $(TARGET_32))
endif
ifeq ($(strip $(CC_64)),)
	$(error 64-bit cross compiler not found for $(TARGET_64))
endif

AS_32:=/usr/bin/nasm
AS_64:=$(AS_32)

LINK_32:=$(CC_32)
LINK_64:=$(CC_64)

PROJECT_ROOT=.
BUILD_ROOT:=$(PROJECT_ROOT)/build
SRC_32_ROOT=$(PROJECT_ROOT)/boot
SRC_64_ROOT=$(PROJECT_ROOT)/src
COMMON_ROOT=$(PROJECT_ROOT)/common
SRC_32_GEN_ROOT = $(BUILD_ROOT)/$(SRC_32_ROOT)
SRC_64_GEN_ROOT = $(BUILD_ROOT)/$(SRC_64_ROOT)
BIN_32_ROOT = $(BUILD_ROOT)/bin_32
BIN_64_ROOT = $(BUILD_ROOT)/bin
ISO_ROOT:=$(BUILD_ROOT)/iso
GRUB_ROOT:=$(ISO_ROOT)/boot/grub

GRUB_CFG:=$(GRUB_ROOT)/grub.cfg
ISO_IMG:=$(BUILD_ROOT)/knud_os.IMG

INCLUDE_32:= -I$(COMMON_ROOT)/include
INCLUDE_32 += $(addprefix -I,$(shell find $(SRC_32_ROOT) -type d -name 'include'))
INCLUDE_64:= -I$(COMMON_ROOT)/include
INCLUDE_64 += $(addprefix -I,$(shell find $(SRC_64_ROOT) -type d -name 'include'))

CPPFLAGS_32:=$(INCLUDE_32) -P 
CPPFLAGS_64:=$(INCLUDE_64) -P
AFLAGS_32:=-felf32 -F dwarf -g
AFLAGS_64:=-felf64 -F dwarf -g
CFLAGS_32:=$(INCLUDE_32) -Wall -Wextra -ffreestanding -fno-pic -fno-pie -masm=intel -gdwarf-4
CFLAGS_64:=$(INCLUDE_64) -mcmodel=large -Wall -Wextra -ffreestanding -fno-pic -fno-pie -masm=intel -gdwarf-4
LFLAGS_32:=-ffreestanding -nostdlib -static -lgcc
LFLAGS_64:=-ffreestanding -nostdlib -static -lgcc

SRC_32_PP_IN = $(shell find $(SRC_32_ROOT) -name '*.$(PP_EXTN)')
SRC_32_PP_OUT = $(patsubst $(SRC_32_ROOT)/%.$(PP_EXTN),$(SRC_32_GEN_ROOT)/%,$(SRC_32_PP_IN))
SRC_32_RAW_IN = $(shell find $(SRC_32_ROOT) -type f $(foreach tmp,$(SRC_32_RAW_EXTNS),-name '*.$(tmp)' -or) -not -name '*')  
SRC_32_RAW_OUT = $(patsubst $(SRC_32_ROOT)/%,$(SRC_32_GEN_ROOT)/%,$(SRC_32_RAW_IN))
SRC_32_ALL = $(SRC_32_PP_OUT) $(SRC_32_RAW_OUT)
OBJ_32_ALL = $(patsubst $(SRC_32_GEN_ROOT)/%,$(BIN_32_ROOT)/%.32.o,$(filter %.c %.S,$(SRC_32_ALL)))

SRC_64_PP_IN = $(shell find $(SRC_64_ROOT) -name '*.$(PP_EXTN)')
SRC_64_PP_OUT = $(patsubst $(SRC_64_ROOT)/%.$(PP_EXTN),$(SRC_64_GEN_ROOT)/%,$(SRC_64_PP_IN))
SRC_64_RAW_IN = $(shell find $(SRC_64_ROOT) -type f $(foreach tmp, $(SRC_64_RAW_EXTNS),-name '*.$(tmp)' -or) -not -name '*')
SRC_64_RAW_OUT = $(patsubst $(SRC_64_ROOT)/%,$(SRC_64_GEN_ROOT)/%,$(SRC_64_RAW_IN))
SRC_64_ALL = $(SRC_64_PP_OUT) $(SRC_64_RAW_OUT)
OBJ_64_ALL = $(patsubst $(SRC_64_GEN_ROOT)/%,$(BIN_64_ROOT)/%.64.o,$(filter %.c %.S,$(SRC_64_ALL)))

CLANG_FLAGS_FILE = $(PROJECT_ROOT)/compile_flags.txt

#TODO: preprocess common if necessary

#main targets
LOADER_BIN := $(BIN_32_ROOT)/load_32.BIN
KERNEL_BIN := $(BIN_64_ROOT)/kernel.BIN

.PHONY: all preprocess clean clang_format

all: $(ISO_IMG) 

preprocess: $(SRC_32_ALL) $(SRC_64_ALL)

clang_format:
	find $(PROJECT_ROOT) -type d -name 'include' -printf "-I%p\n" | sort > $(CLANG_FLAGS_FILE)

$(LOADER_BIN): preprocess $(OBJ_32_ALL)
	$(eval SCRIPT = $(strip $(word 1,$(filter %.ld,$(SRC_32_ALL)))))
	$(LINK_32) $(LFLAGS_32) -T '$(SCRIPT)' $(OBJ_32_ALL) -o $@ -lgcc

$(KERNEL_BIN): preprocess $(OBJ_64_ALL)
	$(eval SCRIPT = $(strip $(word 1,$(filter %.ld,$(SRC_64_ALL)))))
	$(LINK_64) $(LFLAGS_64) -T '$(SCRIPT)' $(OBJ_64_ALL) -o $@ -lgcc

# get sources 
$(SRC_32_GEN_ROOT)/%: $(SRC_32_ROOT)/%
	mkdir -p $(dir $@)
	cp $< $@

$(SRC_32_GEN_ROOT)/%: $(SRC_32_ROOT)/%.ld.$(PP_EXTN)
	mkdir -p $(dir $@)
	cpp $(CPPFLAGS_32) -DNO_INLINE $< -o $@

$(SRC_32_GEN_ROOT)/%: $(SRC_32_ROOT)/%.S.$(PP_EXTN)
	mkdir -p $(dir $@)
	cpp -DNO_INLINE $(CPPFLAGS_32) $< -o $@

$(SRC_32_GEN_ROOT)/%: $(SRC_32_ROOT)/%.$(PP_EXTN)
	mkdir -p $(dir $@)
	cpp $(CPPFLAGS_32) $< -o $@


$(SRC_64_GEN_ROOT)/%: $(SRC_64_ROOT)/%
	mkdir -p $(dir $@)
	cp $< $@

$(SRC_64_GEN_ROOT)/%: $(SRC_64_ROOT)/%.$(PP_EXTN)
	mkdir -p $(dir $@)
	cpp $(CPPFLAGS_64) $< -o $@

# build sources
$(BIN_32_ROOT)/%.c.32.o: $(SRC_32_GEN_ROOT)/%.c
	mkdir -p $(dir $@)
	$(CC_32) $(CFLAGS_32) -c $< -o $@

$(BIN_32_ROOT)/%.S.32.o: $(SRC_32_GEN_ROOT)/%.S
	mkdir -p $(dir $@)
	$(AS_32) $(AFLAGS_32) $< -o $@

$(BIN_64_ROOT)/%.c.64.o: $(SRC_64_GEN_ROOT)/%.c
	mkdir -p $(dir $@)
	$(CC_64) $(CFLAGS_64) -c $< -o $@

$(BIN_64_ROOT)/%.S.64.o: $(SRC_64_GEN_ROOT)/%.S
	mkdir -p $(dir $@)
	$(AS_64) $(AFLAGS_64) $< -o $@

# make iso image
$(ISO_IMG): $(GRUB_CFG) $(LOADER_BIN) $(KERNEL_BIN)
	cp $(LOADER_BIN) $(ISO_ROOT)/boot
	cp $(KERNEL_BIN) $(ISO_ROOT)/boot
	/usr/bin/grub-mkrescue $(ISO_ROOT) -o $@	

# load grub config into iso
$(GRUB_CFG): $(LOADER_BIN) $(KERNEL_BIN)
	mkdir -p $(GRUB_ROOT)
	echo -e "menuentry \"KnudOS\" {" > $(GRUB_CFG)
	echo -e "\tmultiboot /boot/$(notdir $(LOADER_BIN))" >> $(GRUB_CFG)
	echo -e "\tmodule /boot/$(notdir $(KERNEL_BIN)) KnudKernel" >> $(GRUB_CFG)
	echo -e "}" >> $(GRUB_CFG)

# launch & listen on localhost for GDB
qemu: $(ISO_IMG)
	qemu-system-x86_64 -m 8G -cdrom $(ISO_IMG) -gdb tcp::26000 -S


clean:
	rm -rf $(BUILD_ROOT)

