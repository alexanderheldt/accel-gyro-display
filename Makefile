# https://gist.github.com/triffid/b4ccdc0bd00c6ef5d79355aafcb19766
PROJECT:=accel-gyro-display

CLOCK=16000000L
MCU_TARGET=attiny85
ATTINY = /home/alex/code/hobby/avr/ATTinyCore/avr
AVRDUDECONF = $(ATTINY)/avrdude.conf

PROGID = stk500v1
PROGPORT = /dev/ttyACM0
PROGBAUD = 19200

AVRDUDE=avrdude
AVRSIZE=avr-size
AS=avr-gcc
CC=avr-gcc
CXX=avr-g++
LD=avr-g++

CFLAGS+=-Os
CFLAGS+=-DF_CPU=$(CLOCK)
CFLAGS+=-mmcu=$(MCU_TARGET)
CFLAGS+=-funsigned-char -fno-stack-protector
#CFLAGS+=-ffunction-sections -fdata-sections -Wl,--gc-sections -lm
CFLAGS+=-ffunction-sections -fdata-sections -Wl,--gc-sections,-u,vfprintf -lprintf_flt -lm
CFLAGS+=$(patsubst %,-I%,$(SRCPATHS))

OBJS=	$(patsubst %.cpp,$(O)/%.cpp.o, $(wildcard *.cpp)) \
	$(patsubst %.c,$(O)/%.c.o, $(wildcard *.c)) \
	$(patsubst %.S,$(O)/%.S.o, $(wildcard *.S))

O:=build

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

$(O)/$(PROJECT).elf: $(OBJS) | $(O)
	@echo "  LINK  " "$@"
	@$(LD) $(CFLAGS) -o $@ $^
	@$(AVRSIZE) -C --mcu=${MCU_TARGET} $@

$(O)/%.cpp.o: %.cpp | $(O)
	@echo "  CXX   " "$@"
	@$(CXX) $(CFLAGS) -c -o $@ $<

$(O)/%.c.o: %.c | $(O)
	@echo "  CC    " "$@"
	@$(CC) $(CFLAGS) -c -o $@ $<

$(O)/%.S.o: %.S | $(O)
	@echo "  ASM   " "$@"
	@$(AS) $(CFLAGS) -c -o $@ $<
