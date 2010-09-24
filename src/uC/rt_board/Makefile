PROGRAM=rt_board
OUT=$(PROGRAM).out
SRC=$(wildcard *.c)
MCU_TYPE=atmega8


OBJ=$(SRC:.c=.o)
HEX_FILE=$(PROGRAM).hex
BIN_FILE=$(PROGRAM).bin

CC=avr-gcc
OBJ_CP=avr-objcopy

CFLAGS_COMMON=-Wall -mmcu=$(MCU_TYPE) -std=c99
CFLAGS+=$(CFLAGS_COMMON)


.PHONY: all
all: release

.PHONY: debug
debug: CFLAGS+=-g3
debug: hex bin

.PHONY: release
release: CFLAGS+=-O3 -DNDEBUG
release: hex bin

.PHONY: hex
hex: $(HEX_FILE)

.PHONY: bin
bin: $(BIN_FILE)

$(HEX_FILE): $(OUT)
	$(OBJ_CP) -O ihex $< $@

$(BIN_FILE): $(OUT)
	$(OBJ_CP) -O binary $< $@

$(OUT): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@


.PHONY: flash_mcu
flash_mcu:
	avrdude -P avrdoper -c stk500v2 -p m8 -U flash:w:$(PROGRAM).bin

.PHONY: clean
clean:
	rm -fv $(OUT) $(BIN_FILE) $(HEX_FILE) $(OBJ)
