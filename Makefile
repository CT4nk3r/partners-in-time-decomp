# Mario & Luigi: Partners in Time — Decompilation Build System
#
# This Makefile builds the decompiled ARM9 sources into arm9.bin.
# Requires: devkitARM (arm-none-eabi toolchain)
#
# Usage:
#   make              Build build/arm9.bin
#   make clean        Remove build artifacts
#   make verify       Compare build/arm9.bin size with original

# ============================================================================
# Configuration
# ============================================================================

# devkitPro paths
DEVKITPRO ?= /opt/devkitpro
ifeq ($(strip $(DEVKITARM)),)
    DEVKITARM := $(DEVKITPRO)/devkitARM
endif

export PATH := $(DEVKITARM)/bin:$(DEVKITPRO)/tools/bin:$(PATH)

# Toolchain
CC      := arm-none-eabi-gcc
AS      := arm-none-eabi-as
LD      := arm-none-eabi-ld
OBJCOPY := arm-none-eabi-objcopy

# Project paths
BUILD_DIR   := build
EXTRACT_DIR := extracted

# ARM9 configuration
ARM9_SRC_DIR     := arm9/src
ARM9_ASM_DIR     := arm9/asm
ARM9_INCLUDE_DIR := arm9/include
ARM9_BUILD_DIR   := $(BUILD_DIR)/arm9

# Compiler flags
ARM9_CFLAGS  := -std=gnu11 -march=armv5te -mtune=arm946e-s -mthumb-interwork \
                -O2 -Wall -Wno-unused-variable -Wno-unused-function \
                -fno-builtin -nostdlib \
                -I$(ARM9_INCLUDE_DIR)
ARM9_ASFLAGS := -march=armv5te -mthumb-interwork -I$(ARM9_ASM_DIR)
ARM9_LDFLAGS := -T config/arm9.ld -nostdlib --no-warn-rwx-segments

# ============================================================================
# Source file lists
# ============================================================================

ARM9_C_SRCS   := $(wildcard $(ARM9_SRC_DIR)/*.c)
ARM9_ASM_SRCS := $(ARM9_ASM_DIR)/crt0.s
ARM9_C_OBJS   := $(ARM9_C_SRCS:$(ARM9_SRC_DIR)/%.c=$(ARM9_BUILD_DIR)/%.o)
ARM9_ASM_OBJS := $(ARM9_BUILD_DIR)/crt0.o
ARM9_OBJS     := $(ARM9_ASM_OBJS) $(ARM9_C_OBJS)

# ============================================================================
# Targets
# ============================================================================

.PHONY: all clean verify arm9

all: arm9

arm9: $(ARM9_BUILD_DIR)/arm9.bin
	@echo "ARM9 binary built: $<"
	@echo "  Size: $$(wc -c < $(ARM9_BUILD_DIR)/arm9.bin) bytes"

# Link ARM9
$(ARM9_BUILD_DIR)/arm9.elf: $(ARM9_OBJS)
	@echo "Linking ARM9..."
	$(LD) $(ARM9_LDFLAGS) -o $@ $^

# Extract raw binary
$(ARM9_BUILD_DIR)/arm9.bin: $(ARM9_BUILD_DIR)/arm9.elf
	$(OBJCOPY) -O binary $< $@

# Compile ARM9 C sources
$(ARM9_BUILD_DIR)/%.o: $(ARM9_SRC_DIR)/%.c | $(ARM9_BUILD_DIR)
	$(CC) $(ARM9_CFLAGS) -c $< -o $@

# Assemble ARM9 CRT0
$(ARM9_BUILD_DIR)/crt0.o: $(ARM9_ASM_DIR)/crt0.s | $(ARM9_BUILD_DIR)
	$(AS) $(ARM9_ASFLAGS) $< -o $@

# Create build directory (cross-platform)
$(ARM9_BUILD_DIR):
ifeq ($(OS),Windows_NT)
	@if not exist "$(subst /,\,$@)" mkdir "$(subst /,\,$@)"
else
	@mkdir -p $@
endif

# ============================================================================
# Utility targets
# ============================================================================

verify: $(ARM9_BUILD_DIR)/arm9.bin
	@echo "=== Build Verification ==="
	@echo "  Built:    $$(wc -c < $(ARM9_BUILD_DIR)/arm9.bin) bytes"
	@echo "  Original: $$(wc -c < $(EXTRACT_DIR)/arm9.bin) bytes"
	@if [ "$$(wc -c < $(ARM9_BUILD_DIR)/arm9.bin)" = "$$(wc -c < $(EXTRACT_DIR)/arm9.bin)" ]; then \
		echo "  Size: MATCH"; \
	else \
		echo "  Size: MISMATCH (expected for initial build)"; \
	fi

clean:
ifeq ($(OS),Windows_NT)
	@if exist "$(subst /,\,$(BUILD_DIR))" rmdir /s /q "$(subst /,\,$(BUILD_DIR))"
else
	rm -rf $(BUILD_DIR)
endif
	@echo "Build directory cleaned."
