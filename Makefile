DEVICE = attiny85
CLOCK  = 8000000 # 8MHz

PROGRAMMER = stk500v1
PORT = /dev/ttyACM0

clean.o:
	rm -f main.o

clean.hex:
	rm -f main.hex

main.o: clean.o
	avr-gcc -Wall -Os -mmcu=$(DEVICE) -DF_CPU=$(CLOCK) main.c -o main.o
	avr-size --format=avr --mcu=$(DEVICE) main.o

main.hex: clean.hex main.o
	avr-objcopy -j .text -j .data -O ihex main.o main.hex

flash: main.hex
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b 19200 -U flash:w:main.hex:i

