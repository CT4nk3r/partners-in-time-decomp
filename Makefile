# Mario & Luigi: Partners in Time — Decompilation Build System
#
# This Makefile builds the decompiled sources into a matching NDS ROM.
# Requires: devkitARM (arm-none-eabi-gcc), ndstool, Python 3
#
# Usage:
#   make              Build the ROM
#   make clean        Remove build artifacts
#   make extract      Extract the base ROM
#   make verify       Verify built ROM matches base ROM
#   make setup        Check that all required tools are installed

# ============================================================================
# Configuration
# ============================================================================

# devkitPro paths
ifeq ($(strip $(DEVKITPRO)),)
    $(error "Please set DEVKITPRO in your environment. See SETUP.md")
endif
ifeq ($(strip $(DEVKITARM)),)
    DEVKITARM := $(DEVKITPRO)/devkitARM
endif

export PATH := $(DEVKITARM)/bin:$(DEVKITPRO)/tools/bin:$(PATH)

# Toolchain
CC      := arm-none-eabi-gcc
AS      := arm-none-eabi-as
LD      := arm-none-eabi-ld
OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump
AR      := arm-none-eabi-ar

# Project paths
BASEROM     := rom/baserom.nds
BUILD_DIR   := build
EXTRACT_DIR := extracted

# ARM9 configuration
ARM9_SRC_DIR     := arm9/src
ARM9_ASM_DIR     := arm9/asm
ARM9_INCLUDE_DIR := arm9/include
ARM9_DATA_DIR    := arm9/data
ARM9_BUILD_DIR   := $(BUILD_DIR)/arm9

# ARM7 configuration
ARM7_SRC_DIR     := arm7/src
ARM7_ASM_DIR     := arm7/asm
ARM7_INCLUDE_DIR := arm7/include
ARM7_DATA_DIR    := arm7/data
ARM7_BUILD_DIR   := $(BUILD_DIR)/arm7

# Compiler flags (matching original SDK settings for NDS)
ARM9_CFLAGS  := -std=gnu11 -march=armv5te -mtune=arm946e-s -mthumb-interwork \
                -O2 -Wall -fno-builtin -nostdlib \
                -I$(ARM9_INCLUDE_DIR)
ARM9_ASFLAGS := -march=armv5te -mthumb-interwork
ARM9_LDFLAGS := -T config/arm9.ld -nostdlib

ARM7_CFLAGS  := -march=armv4t -mtune=arm7tdmi -mthumb-interwork \
                -O2 -Wall -fno-builtin -nostdlib \
                -I$(ARM7_INCLUDE_DIR)
ARM7_ASFLAGS := -march=armv4t -mthumb-interwork
ARM7_LDFLAGS := -T config/arm7.ld -nostdlib

# Output
OUTPUT_ROM := $(BUILD_DIR)/mlpit.nds

# ============================================================================
# Source file lists
# ============================================================================

ARM9_C_SRCS   := $(wildcard $(ARM9_SRC_DIR)/*.c)
ARM9_ASM_SRCS := $(wildcard $(ARM9_ASM_DIR)/*.s)
ARM9_C_OBJS   := $(ARM9_C_SRCS:$(ARM9_SRC_DIR)/%.c=$(ARM9_BUILD_DIR)/%.o)
ARM9_ASM_OBJS := $(ARM9_ASM_SRCS:$(ARM9_ASM_DIR)/%.s=$(ARM9_BUILD_DIR)/%.o)
ARM9_OBJS     := $(ARM9_C_OBJS) $(ARM9_ASM_OBJS)

ARM7_C_SRCS   := $(wildcard $(ARM7_SRC_DIR)/*.c)
ARM7_ASM_SRCS := $(wildcard $(ARM7_ASM_DIR)/*.s)
ARM7_C_OBJS   := $(ARM7_C_SRCS:$(ARM7_SRC_DIR)/%.c=$(ARM7_BUILD_DIR)/%.o)
ARM7_ASM_OBJS := $(ARM7_ASM_SRCS:$(ARM7_ASM_DIR)/%.s=$(ARM7_BUILD_DIR)/%.o)
ARM7_OBJS     := $(ARM7_C_OBJS) $(ARM7_ASM_OBJS)

# ============================================================================
# Targets
# ============================================================================

.PHONY: all clean extract verify setup

all: $(OUTPUT_ROM)

# Build the final NDS ROM
$(OUTPUT_ROM): $(ARM9_BUILD_DIR)/arm9.bin $(ARM7_BUILD_DIR)/arm7.bin
	@echo "Building NDS ROM..."
	ndstool -c $@ \
		-9 $(ARM9_BUILD_DIR)/arm9.bin \
		-7 $(ARM7_BUILD_DIR)/arm7.bin \
		-h $(EXTRACT_DIR)/header.bin \
		-t $(EXTRACT_DIR)/banner.bin \
		-d $(EXTRACT_DIR)/data \
		-y $(EXTRACT_DIR)/overlay \
		-y9 $(EXTRACT_DIR)/y9.bin \
		-y7 $(EXTRACT_DIR)/y7.bin
	@echo "ROM built: $@"

# Link ARM9
$(ARM9_BUILD_DIR)/arm9.elf: $(ARM9_OBJS)
	$(LD) $(ARM9_LDFLAGS) -o $@ $^

$(ARM9_BUILD_DIR)/arm9.bin: $(ARM9_BUILD_DIR)/arm9.elf
	$(OBJCOPY) -O binary $< $@

# Link ARM7
$(ARM7_BUILD_DIR)/arm7.elf: $(ARM7_OBJS)
	$(LD) $(ARM7_LDFLAGS) -o $@ $^

$(ARM7_BUILD_DIR)/arm7.bin: $(ARM7_BUILD_DIR)/arm7.elf
	$(OBJCOPY) -O binary $< $@

# Compile ARM9 C sources
$(ARM9_BUILD_DIR)/%.o: $(ARM9_SRC_DIR)/%.c | $(ARM9_BUILD_DIR)
	$(CC) $(ARM9_CFLAGS) -c $< -o $@

# Assemble ARM9 assembly sources
$(ARM9_BUILD_DIR)/%.o: $(ARM9_ASM_DIR)/%.s | $(ARM9_BUILD_DIR)
	$(AS) $(ARM9_ASFLAGS) $< -o $@

# Compile ARM7 C sources
$(ARM7_BUILD_DIR)/%.o: $(ARM7_SRC_DIR)/%.c | $(ARM7_BUILD_DIR)
	$(CC) $(ARM7_CFLAGS) -c $< -o $@

# Assemble ARM7 assembly sources
$(ARM7_BUILD_DIR)/%.o: $(ARM7_ASM_DIR)/%.s | $(ARM7_BUILD_DIR)
	$(AS) $(ARM7_ASFLAGS) $< -o $@

# Create build directories
$(ARM9_BUILD_DIR) $(ARM7_BUILD_DIR):
	@mkdir -p $@

# ============================================================================
# Utility targets
# ============================================================================

extract:
	python tools/scripts/extract_rom.py $(BASEROM)

verify: $(OUTPUT_ROM)
	python tools/scripts/verify_match.py

setup:
	@echo "Checking required tools..."
	@which arm-none-eabi-gcc > /dev/null 2>&1 && echo "  ✓ arm-none-eabi-gcc" || echo "  ✗ arm-none-eabi-gcc (install devkitARM)"
	@which ndstool > /dev/null 2>&1 && echo "  ✓ ndstool" || echo "  ✗ ndstool (install devkitPro)"
	@which python3 > /dev/null 2>&1 && echo "  ✓ python3" || echo "  ✗ python3"
	@which make > /dev/null 2>&1 && echo "  ✓ make" || echo "  ✗ make"
	@echo ""
	@test -f $(BASEROM) && echo "  ✓ Base ROM found" || echo "  ✗ Base ROM not found (place at $(BASEROM))"

clean:
	rm -rf $(BUILD_DIR)
	@echo "Build directory cleaned."
