#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#

.SUFFIXES:

PROJECT		:= GSP

TARGET := release # can be release or debug

LIBSEVEN	:= ./externals/libseven
MINRT		:= ./externals/libseven/gba-minrt

SOURCES		:= rt/crt0.s $(shell find ./src -name '*.c') $(shell find ./src -name '*.s')
INCLUDES	:= #
LIBDIRS		:= $(LIBSEVEN)
LIBS		:= seven

BUILDDIR	:= build

FLAGS := -O2 -std=c99 -ffunction-sections -fdata-sections

# TARGET Specific
FLAGS.release :=
FLAGS.debug := -g3 -gdwarf-4 -DNDEBUG

CFLAGS		:= $(FLAGS.$(TARGET)) $(FLAGS)
LDFLAGS		:= -mthumb -nostartfiles -specs=nano.specs -specs=nosys.specs -Wl,-Trom.ld -L$(MINRT)/rt

#
# Internal
#

vpath rt/% $(MINRT)

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

ELFFILE = $(BUILDDIR)/$(PROJECT).elf
ROMFILE = $(BUILDDIR)/$(PROJECT).gba
MAPFILE = $(BUILDDIR)/$(PROJECT).map

LIBSEVEN_LIB = $(LIBSEVEN)/lib/libseven.a

OBJECTS = $(SOURCES:%=$(BUILDDIR)/obj/%.o)
DEPENDS = $(SOURCES:%=$(BUILDDIR)/dep/%.d)
OBJDIRS = $(dir $(BUILDDIR) $(OBJECTS) $(DEPENDS))

CFLAGS += \
	  -mcpu=arm7tdmi \
	  -mabi=aapcs \
	  -mthumb \
	  $(LIBDIRS:%=-I%/include) \
	  $(INCLUDES:%=-I%) \

LDFLAGS += \
	   -Wl,--gc-sections \
	   -Wl,-Map,$(MAPFILE) \
	   $(LIBDIRS:%=-L%/lib) \
	   $(LIBS:%=-l%) \

$(ROMFILE): $(ELFFILE)
$(ELFFILE): $(OBJECTS) $(LIBSEVEN_LIB)
$(OBJECTS): | builddirs

%.gba:
	@echo "$@"
	@$(OBJCOPY) -O binary $< $@

%.elf:
	@echo "$@"
	@$(CC) -o $@ $^ $(LDFLAGS)

$(BUILDDIR)/obj/%.o: %
	@echo "$<"
	@$(CC) -c -o $@ $(CFLAGS) -MMD -MP -MF $(BUILDDIR)/dep/$<.d $<

$(LIBSEVEN_LIB):
	@$(MAKE) -C $(LIBSEVEN)

builddirs:
	@mkdir -p $(OBJDIRS)

clean:
	@echo "clean"
	@rm -rf $(BUILDDIR)

.PHONY: libseven builddirs clean

-include $(DEPENDS)
