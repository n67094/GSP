.SUFFIXES:

# tricks
define newline


endef

# Path to gba-minrt
MINRT := ./externals/libseven/gba-minrt

# Path to libseven
LIBSEVEN := ./externals/libseven

# Name of your ROM
PROJECT := GSP

# Source files
SOURCES := $(MINRT)/src/crt0.s $(shell find ./src ./data -type f -name '*.c' -o -name '*.s' ! -path '*/.*')

# Include directories
INCLUDES := $(MINRT)/src

# Library directories, with /include and /lib
LIBDIRS := $(MINRT) $(LIBSEVEN)

# Libraries to link
LIBS := seven

# All build output goes here
BUILDDIR := build

# Used to convert bin files to .s and .h with bin2s
BIN2S := ./tools/bin2s/bin2s
DATA := $(shell find data -type f -name '*.*' ! -name '*.c' ! -name '*.h' ! -name '*.s' ! -path '*/.*')

# C compiler flags
TARGET := release

FLAGS := -O2 -std=c99 -ffunction-sections -fdata-sections
FLAGS.release :=
FLAGS.debug := -g3 -gdwarf-4 -DNDEBUG

CFLAGS := $(FLAGS.$(TARGET)) $(FLAGS)

# Linker flags
LDFLAGS := -mthumb -nostartfiles \
		   -specs=nano.specs -specs=nosys.specs \
		   -Wl,-Trom.ld

# Toolchain prefix
#
# Only change this if you want to use a different compiler
TOOLCHAIN	:= arm-none-eabi

# Uncomment this if you want to use C++
#
# USE_CXX := yes

# Uncomment this if you want to use Link Time Optimization
#
# USE_LTO := yes

#
# Internal
#

CC := $(TOOLCHAIN)-gcc
CXX := $(TOOLCHAIN)-g++
OBJCOPY := $(TOOLCHAIN)-objcopy
LD := $(if $(USE_CXX),$(CXX),$(CC))

ELFFILE	:= $(BUILDDIR)/$(PROJECT).elf
ROMFILE	:= $(BUILDDIR)/$(PROJECT).gba
MAPFILE	:= $(BUILDDIR)/$(PROJECT).map

default: $(ROMFILE)

CFLAGS += \
	-mcpu=arm7tdmi \
	-mabi=aapcs \
	-mthumb \
	$(LIBDIRS:%=-I%/include) \
	$(INCLUDES:%=-I%) \
	$(if $(USE_LTO),-flto,-fno-lto) \

LDFLAGS += \
	-Wl,--gc-sections \
	-Wl,-Map,$(MAPFILE) \
	$(LIBDIRS:%=-L%/lib) \
	$(LIBS:%=-l%) \
	$(if $(USE_LTO),-flto,-fno-lto) \

OBJECTS :=
DEPENDS :=

SPACE := $(subst ,, )

eq = $(and $(findstring x$(1),x$(2)),$(findstring x$(2),x$(1)))

pathmap = $(2)$(subst $(SPACE),/,$(foreach component,$(subst /, ,$(1)),$(if $(call eq,$(component),..),__,$(component))))$(3)

obj = $(call pathmap,$(1),$(BUILDDIR)/obj/,.o)
dep = $(call pathmap,$(1),$(BUILDDIR)/dep/,.d)

define compile =
$(call obj,$(1)): $(1)

	@$$(CC) -c -o $$@ $$(CFLAGS) -MMD -MP -MF $(call dep,$(1)) $$<

OBJECTS += $(call obj,$(1))
DEPENDS += $(call dep,$(1))
endef

$(foreach source,$(SOURCES),$(eval $(call compile,$(source))))

DIRS := $(dir $(BUILDDIR) $(OBJECTS) $(DEPENDS))

$(ROMFILE): $(ELFFILE)
$(ELFFILE): $(OBJECTS)
$(OBJECTS): | dirs

%.elf:
	@$(LD) -o $@ $^ $(LDFLAGS)

%.gba:
	@$(OBJCOPY) -O binary $< $@

bin:
	$(foreach file, $(DATA), $(BIN2S) -a 4 -H $(file).h $(file) > $(file).s $(newline))

dirs:
	@mkdir -p $(DIRS)

clean:
	@rm -rf $(BUILDDIR)

run: $(ELFFILE)
	@mgba-qt $(ELFFILE)

.PHONY: default dirs clean run

-include $(DEPENDS)

