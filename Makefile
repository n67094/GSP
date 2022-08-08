.SUFFIXES:

PROJECT := GSP

TARGET := release # can be release or debug

LIBSEVEN := ./externals/libseven
MINRT := ./externals/libseven/gba-minrt

LIBDIRS := $(LIBSEVEN)
LIBS := seven

BIN2S := ./externals/bin2s/bin2s #bin2s exec

SOURCES := rt/crt0.s $(shell find ./src ./data -type f -name '*.c' -o -name '*.s' ! -path '*/.*')
INCLUDES :=
DATA := $(shell find ./data -type f -name '*.*' ! -name '*.c' ! -name '*.h' ! -name '*.s' ! -path '*/.*')

BUILDDIR := build

FLAGS := -O2 -std=c99 -ffunction-sections -fdata-sections

# TARGET Specific
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
BINARIES = $(DATA:%=/data/%)
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
$(OBJECTS): $(BINARIES)
$(BINARIES): | builddirs

%.gba:
	@echo "1. $@"
	@$(OBJCOPY) -O binary $< $@

%.elf:
	@echo "2. $@"
	@$(CC) -o $@ $^ $(LDFLAGS)

$(BUILDDIR)/obj/%.o: %
	@echo "3. $@"
	@$(CC) -c -o $@ $(CFLAGS) -MMD -MP -MF $(BUILDDIR)/dep/$<.d $<

$(LIBSEVEN_LIB):
	@echo "4. $@"
	@$(MAKE) -C $(LIBSEVEN)

builddirs:
	@echo "5. $@"
	@mkdir -p $(OBJDIRS)

/data/%: %
	@echo "6. $@"
	$(BIN2S) -a 4 -H $<.h $< > $<.s

bin2o:
	@echo "7. $@"
	@$(foreach file, $(DATA), bin2s -a 4 -H $(file))

clean:
	@echo "clean"
	@rm -rf $(BUILDDIR)

.PHONY: libseven builddirs clean

-include $(DEPENDS)
