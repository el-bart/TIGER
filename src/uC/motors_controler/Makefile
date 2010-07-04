PROGRAM=motors_control
SRCS=$(shell find . -type f -name '*.c')
MCU_TYPE=attiny2313


OBJS=$(SRCS:.c=.o)
HEX_FILE=$(PROGRAM).hex

CC=avr-gcc
OBJ_CP=avr-objcopy

CFLAGS_COMMON=-Wall -mmcu=$(MCU_TYPE)
CFLAGS+=$(CFLAGS_COMMON)


.PHONY: all
all: debug

.PHONY: debug
debug: CFLAGS+=-g3 -DDEBUG
debug: hex

.PHONY: release
release: CFLAGS+=-O3 -DNDEBUG
release: hex

.PHONY: hex
hex: $(HEX_FILE)


$(HEX_FILE): $(PROGRAM)
	$(OBJ_CP) -O ihex $(PROGRAM) $(HEX_FILE)

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) $< -o $(PROGRAM)


.PHONY: flash_mcu
flash_mcu:
	uisp -dprog=stk200 --erase --upload if=$(HEX_FILE) --verify

.PHONY: clean
clean:
	rm -fv $(PROGRAM) $(HEX_FILE) $(OBJS)

