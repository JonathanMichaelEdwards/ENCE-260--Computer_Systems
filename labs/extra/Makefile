# File:   Makefile
# Author: Jonathan Edwards
# Date:   3/10/2018
# Descr:  Makefile for practice

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I. -I../../drivers
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: main.out


# Compile: create object files from C source files.
main.o: main.c
	$(CC) -c $(CFLAGS) $< -o $@


# Link: create ELF output file from object files.
main.out: main.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: main.out
	$(OBJCOPY) -O ihex main.out main.hex
	dfu-programmer atmega32u2 erase; 
	dfu-programmer atmega32u2 flash main.hex; 
	dfu-programmer atmega32u2 start


