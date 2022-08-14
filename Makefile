.SUFFIXES:

PROJECT := GSP

LIBSEVEN := ./externals/libseven
MINRT := ./externals/libseven/gba-minrt

LIBDIRS := $(LIBSEVEN)
LIBS := seven

BUILDDIR := build

# Data folder can containes sources which was note generate from bin2s
SOURCES := rt/crt0.s $(shell find ./src ./data -type f -name '*.c' -o -name '*.s' ! -path '*/.*')
INCLUDES :=

# Used to convert bin files to .s and .h with bin2s
BIN2S := ./tools/bin2s/bin2s
DATA := $(shell find ./data -type f -name '*.*' ! -name '*.c' ! -name '*.h' ! -name '*.s' ! -path '*/.*')
BINARIES = $(DATA:%=/data/%)

# Target specific
TARGET := release # can be release or debug

FLAGS := -O2 -std=c99 -ffunction-sections -fdata-sections
FLAGS.release :=
FLAGS.debug := -g3 -gdwarf-4 -DNDEBUG

CFLAGS := $(FLAGS.$(TARGET)) $(FLAGS)
LDFLAGS := -mthumb -nostartfiles -specs=nano.specs -specs=nosys.specs -Wl,-Trom.ld -L$(MINRT)/rt

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
$(OBJECTS): | builddirs $(BINARIES)

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

/data/%: %
	$(BIN2S) -a 4 -H $<.h $< > $<.s

bin2o:
	@$(foreach file, $(DATA), bin2s -a 4 -H $(file))

clean:
	@echo "clean"
	@rm -rf $(BUILDDIR)

.PHONY: libseven builddirs clean

-include $(DEPENDS)
