# name of the application
export PROGRAM_NAME:=rt_board

# toolchain
export CC :=avr-gcc
export CXX:=avr-g++
export OBJCOPY:=avr-objcopy
export LD :=$(CC)

# tools settings
export MCU_TYPE:=atmega8

# extra flags, if needed
export CFLAGS  +=-mmcu=$(MCU_TYPE) -std=c99
export CXXFLAGS+=-mmcu=$(MCU_TYPE) -std=c99
export LDFLAGS +=-mmcu=$(MCU_TYPE) -std=c99
