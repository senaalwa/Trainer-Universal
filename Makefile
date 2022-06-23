MAIN_APP = main

CC = avr-gcc

OBJ = avr-objcopy 

SIZE = avr-size

CC_FLAG = -Wall -Wextra -Os -g -mmcu=atmega16 -o

OBJ_FLAG = -O ihex ${MAIN_APP}.elf ${MAIN_APP}.hex

SIZE_FLAG = -A

AVRDUDE = -c usbasp -p m16 -P usb -b 115200 -B 46.88 -U flash:w:${MAIN_APP}.hex

SRC = ${MAIN_APP}.c


# Make does not offer a recursive wildcard function, so here's one:
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# How to recursively find all files that match a pattern
ALL_C := $(call rwildcard,./lib/,*.c)
ALL_HEADER := $(sort $(dir $(wildcard ./lib/*/)))
SRC += ${ALL_C} 

size: compile
	${SIZE} ${SIZE_FLAG} ${MAIN_APP}.elf

compile: build
	${OBJ} ${OBJ_FLAG}

build:
	${CC} $(addprefix -I, ${ALL_HEADER}) ${CC_FLAG} ${MAIN_APP}.elf ${SRC}

clean:
	rm ${MAIN_APP}.elf ${MAIN_APP}.hex

upload: size
	avrdude ${AVRDUDE}

