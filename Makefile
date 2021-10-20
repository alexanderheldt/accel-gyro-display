# https://gist.github.com/triffid/b4ccdc0bd00c6ef5d79355aafcb19766
PROJECT:=accel-gyro-display

CLOCK=8000000L
MCU_TARGET=attiny85
ATTINY = /home/alex/code/hobby/avr/ATTinyCore/avr
ATTINY_VARIANT = $(ATTINY)/variants/tinyX5
ATTINY_CORE = $(ATTINY)/cores/tiny
LIBS = $(ATTINY)/libraries

AVRDUDECONF = $(ATTINY)/avrdude.conf

PROGID = stk500v1
PROGPORT = /dev/ttyACM0
PROGBAUD = 19200

O:=build

AVRDUDE=avrdude
AS=avr-gcc
CC=avr-gcc
CXX=avr-g++
LD=avr-g++

SRCPATHS=$(ATTINY_CORE) $(ATTINY_VARIANT) \
	 $(LIBS)/SoftwareSerial

CFLAGS+=-Os
CFLAGS+=-DF_CPU=$(CLOCK)
CFLAGS+=-mmcu=$(MCU_TARGET)
CFLAGS+=-funsigned-char -fno-stack-protector
CFLAGS+=-ffunction-sections -fdata-sections -Wl,--gc-sections
CFLAGS+=$(patsubst %,-I%,$(SRCPATHS))

VPATH=$(foreach dir,$(SRCPATHS),$(shell find -H $(dir) -type d))

CXXSRC=$(wildcard *.cpp) $(foreach dir,$(SRCPATHS),$(shell find -H $(dir) -name '*.cpp'))
CSRC=$(wildcard *.c) $(foreach dir,$(SRCPATHS),$(shell find -H $(dir) -name '*.c'))
ASRC=$(wildcard *.S) $(foreach dir,$(SRCPATHS),$(shell find -H $(dir) -name '*.S'))

OBJS=	$(patsubst %.cpp,$(O)/%.cpp.o, $(notdir $(CXXSRC))) \
	$(patsubst %.c,$(O)/%.c.o, $(notdir $(CSRC))) \
	$(patsubst %.S,$(O)/%.S.o, $(notdir $(ASRC)))

OBJ=$(patsubst $(O)/main.cpp.o,,$(OBJS))

.PHONY: all clean flash

all: $(O)/$(PROJECT).hex

clean:
	rm -r $(O)

flash: $(O)/$(PROJECT).hex
	$(AVRDUDE) -c$(PROGID) -b$(PROGBAUD) -p$(MCU_TARGET) -P$(PROGPORT) -C$(AVRDUDECONF) -U flash:w:$<

$(O):
	@mkdir -p "$@"

%.hex: %.elf
	@echo "  HEX   "  "$@"
	@$(PREFIX)objcopy -j .text -j .data -O ihex $< $@

$(O)/$(PROJECT).elf: $(OBJ) | $(O)
	@echo "  LINK  " "$@"
	@$(LD) $(CFLAGS) -o $@ $^

$(O)/%.cpp.o: %.cpp | $(O)
	@echo "  CXX   " "$@"
	@$(CXX) $(CFLAGS) -c -o $@ $<

$(O)/%.c.o: %.c | $(O)
	@echo "  CC    " "$@"
	@$(CC) $(CFLAGS) -c -o $@ $<

$(O)/%.S.o: %.S | $(O)
	@echo "  ASM   " "$@"
	@$(AS) $(CFLAGS) -c -o $@ $<
