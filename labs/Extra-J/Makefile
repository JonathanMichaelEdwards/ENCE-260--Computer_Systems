# File:   Makefile
# Author: M. P. Hayes, UCECE
# Date:   12 Sep 2010
# Descr:  Makefile for practice

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I. -I../../drivers/avr -I../../drivers
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: practice.out


# Compile: create object files from C source files.
practice.o: practice.c pacer.h
	$(CC) -c $(CFLAGS) $< -o $@

pio.o: ../../drivers/avr/pio.c ../../drivers/avr/pio.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../../drivers/avr/system.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

led.o: ../../drivers/led.c ../../drivers/led.h
	$(CC) -c $(CFLAGS) $< -o $@

pacer.o: pacer.c pacer.h
	$(CC) -c $(CFLAGS) $< -o $@



# Link: create ELF output file from object files.
practice.out: practice.o pio.o system.o led.o pacer.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: practice.out
	$(OBJCOPY) -O ihex practice.out practice.hex
	dfu-programmer atmega32u2 erase; 
	dfu-programmer atmega32u2 flash practice.hex; 
	dfu-programmer atmega32u2 start


