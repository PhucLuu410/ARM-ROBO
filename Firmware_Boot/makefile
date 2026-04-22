TARGET = boot

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

CFLAGS = -mcpu=cortex-m3 -mthumb -Wall -O0 -g \
         -ffunction-sections -fdata-sections \
         -Isystem/CMSIS

LDFLAGS = -T system/linker/STM32F103C8T6_Linker.ld \
          -nostartfiles \
          -Wl,--gc-sections \
          -Wl,-Map=$(TARGET).map

SRC = main.c system/startup/STM32F103C8T6_StartUp.c

OBJ = $(SRC:.c=.o)

all: $(TARGET).bin

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@
	$(SIZE) $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

flash: $(TARGET).bin
	openocd -f interface/stlink.cfg \
	       -f target/stm32f1x.cfg \
	       -c "program $(TARGET).bin 0x08000000 verify reset exit"

clean:
	rm -f *.o *.elf *.bin *.map