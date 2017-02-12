.PHONY: all
all: main.elf main.bin main.hex

FW:=../../firmware/STM32F0-Discovery_FW_V1.0.0
STINC:=$(FW)/Libraries/CMSIS/ST/STM32F0xx/Include
CMINC:=$(FW)/Libraries/CMSIS/Include/

FC := -Wall
FC += -gdwarf-2 -mthumb -mcpu=cortex-m0
FC += -I$(STINC) -I$(CMINC)
#FC += -O3 -Os
FC += -g3 -ggdb

FCC := $(FC)

FXX := $(FC)
FXX += -fno-exceptions -fno-rtti
FXX += -std=c++11

FL := $(FC)
FL += -nostartfiles -Tstm32f0_linker.ld
FL += -flto

main.elf: main.o startup_stm32f0xx.o system_stm32f0xx.o
	arm-none-eabi-g++ -o $@ $^ $(FL)
main.bin: main.elf Makefile
	arm-none-eabi-objcopy -O binary -S $< $@
main.hex: main.elf Makefile
	arm-none-eabi-objcopy -O ihex   -S $< $@

%.o: %.s Makefile
	arm-none-eabi-gcc -x assembler-with-cpp -c $< -o $@ $(FCC)

%.o: %.c Makefile
	arm-none-eabi-gcc -o $@ -c $< $(FCC)

%.o: %.cpp Makefile
	arm-none-eabi-g++ -o $@ -c $< $(FXX)

.PHONY: clean
clean:
	rm -vf main.elf main.hex main.bin *.o


.PHONY: size
size: main.elf
	arm-none-eabi-size $<

.PHONY: flash
flash: main.bin
	st-flash --reset write $< 0x8000000

.PHONY: debug
debug: main.elf
	xterm -e st-util &
	arm-none-eabi-gdb --tui --eval-command="target remote localhost:4242" $< -ex 'load'
