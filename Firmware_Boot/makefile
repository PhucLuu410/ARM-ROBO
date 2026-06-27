TARGET = boot

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

CFLAGS = -mcpu=cortex-m3 -mthumb -Wall -O0 -g \
         -ffunction-sections -fdata-sections \
         -Isystem/CMSIS \
         -IApp \
         -IBsp

LDFLAGS = -T system/linker/STM32F103C8T6_Linker.ld \
          -nostartfiles \
          -Wl,--gc-sections \
          -Wl,-Map=system/build/$(TARGET).map

SRC = 	main.c \
		system/startup/STM32F103C8T6_StartUp.c \
		App/Com.c \
		App/Firmware.c \
		App/State_Machine.c \
		App/System.c \
		Bsp/Uart.c \
		Bsp/Flash.c \
		Bsp/Dio.c	\
		Bsp/Port.c	\
		Bsp/Rcc.c \


OBJ = $(SRC:.c=.o)

all: system/build/$(TARGET).bin
	g++ Load_Firmware.cpp -o Load_Firmware.exe


system/build/$(TARGET).bin: system/build/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

system/build/$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@
	$(SIZE) $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

flash: system/build/$(TARGET).bin
	openocd -f interface/stlink.cfg \
	       -f target/stm32f1x.cfg \
	       -c "program system/build/$(TARGET).bin 0x08000000 verify reset exit"

.PHONY: clean
clean:
	del /q /f *.o 2>nul || exit 0
	del /q /f system\startup\*.o 2>nul || exit 0
	del /q /f App\*.o 2>nul || exit 0
	del /q /f Bsp\*.o 2>nul || exit 0
	del /q /f system\build\* 2>nul || exit 0