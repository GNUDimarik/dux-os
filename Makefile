# ============================================================================
# DUX
# Simple monolithic build
# ============================================================================

ARCH          ?= x86
TARGET        := i686-elf

CXX           := $(TARGET)-g++
CC            := $(TARGET)-gcc
AS            := $(TARGET)-gcc

BUILD_DIR     := build
TARGET_ELF    := $(BUILD_DIR)/dux

BOOT_SRC      := arch/$(ARCH)/boot.S
LINKER_SCRIPT := arch/$(ARCH)/linker.ld


# ============================================================================
# Sources
# ============================================================================

# All C++ sources in the project.
CPP_SOURCES := $(shell find . \
	-type f \
	-name '*.cpp' \
	-not -path './build/*' \
	-not -path './build-*/*')

# Preserve the source directory hierarchy inside build/.
#
# kernel/main.cpp       -> build/kernel/main.o
# arch/x86/foo.cpp      -> build/arch/x86/foo.o
# drivers/foo/bar.cpp   -> build/drivers/foo/bar.o
CPP_OBJECTS := $(patsubst ./%.cpp,$(BUILD_DIR)/%.o,$(CPP_SOURCES))

# Bootstrap code is handled explicitly.
BOOT_OBJECT := $(BUILD_DIR)/arch/$(ARCH)/boot.o

OBJECTS := $(BOOT_OBJECT) $(CPP_OBJECTS)


# ============================================================================
# Include paths
# ============================================================================

# Project root plus every directory named "include".
#
# Examples:
#   ./include
#   ./arch/x86/include
#   ./kernel/include
#   ./drivers/include
#   ./lib/include
INCLUDE_DIRS := \
	. \
	$(shell find . \
		-type d \
		-name include \
		-not -path './build/*' \
		-not -path './build-*/*')

CPPFLAGS := $(addprefix -I,$(INCLUDE_DIRS))


# ============================================================================
# Compiler flags
# ============================================================================

CXXFLAGS := \
	-std=c++20 \
	-ffreestanding \
	-fno-exceptions \
	-fno-rtti \
	-fno-stack-protector \
	-fno-pie \
	-fno-pic \
	-ffunction-sections \
	-fdata-sections \
	-Wall \
	-Wextra \
	-O2 \
	-g

ASFLAGS := \
	-ffreestanding \
	-fno-pie \
	-fno-pic


# ============================================================================
# Linker flags
# ============================================================================

LDFLAGS := \
	-T $(LINKER_SCRIPT) \
	-nostdlib \
	-static \
	-no-pie \
	-Wl,--gc-sections

LDLIBS := -lgcc


# ============================================================================
# Main targets
# ============================================================================

.PHONY: all clean check print-sources print-includes

all: $(TARGET_ELF)


# ============================================================================
# Final kernel link
# ============================================================================

$(TARGET_ELF): $(OBJECTS) $(LINKER_SCRIPT)
	@mkdir -p $(dir $@)

	$(CXX) \
		$(LDFLAGS) \
		$(OBJECTS) \
		$(LDLIBS) \
		-o $@

	@echo
	@echo "========================================"
	@echo " DUX built successfully"
	@echo " => $@"
	@echo "========================================"


# ============================================================================
# C++ compilation
# ============================================================================

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)

	$(CXX) \
		$(CPPFLAGS) \
		$(CXXFLAGS) \
		-MMD \
		-MP \
		-c $< \
		-o $@


# ============================================================================
# Bootstrap assembly
# ============================================================================

$(BOOT_OBJECT): $(BOOT_SRC)
	@mkdir -p $(dir $@)

	$(AS) \
		$(CPPFLAGS) \
		$(ASFLAGS) \
		-c $< \
		-o $@


# ============================================================================
# Verification
# ============================================================================

check: $(TARGET_ELF)
	@echo
	@echo "ELF:"
	@file $(TARGET_ELF)

	@echo
	@echo "Multiboot2:"
	@grub-file --is-x86-multiboot2 $(TARGET_ELF)
	@echo "Multiboot2: OK"

	@echo
	@echo "ELF header:"
	@readelf -h $(TARGET_ELF)

	@echo
	@echo "Sections:"
	@objdump -h $(TARGET_ELF)


# ============================================================================
# Diagnostics
# ============================================================================

print-sources:
	@echo "C++ sources:"
	@printf '  %s\n' $(CPP_SOURCES)

print-includes:
	@echo "Include directories:"
	@printf '  %s\n' $(INCLUDE_DIRS)


# ============================================================================
# Clean
# ============================================================================

clean:
	rm -rf $(BUILD_DIR)


# ============================================================================
# Generated dependencies
# ============================================================================

-include $(CPP_OBJECTS:.o=.d)