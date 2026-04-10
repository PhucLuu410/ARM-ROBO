    ENTRY(Reset_Handler)

    MEMORY
    {
        FLASH (rx)      : ORIGIN = 0x08001000, LENGTH = 60K
        BUFFER (xrw)     : ORIGIN = 0x20000000, LENGTH = 4K
        RAM (xrw)     : ORIGIN = 0x20001000, LENGTH = 16K
    }

    SECTIONS
    {
        .isr_vector :
        {
            . = ALIGN(4);
            KEEP(*(.isr_vector)) 
            . = ALIGN(4);
            
        } > FLASH

        .text :
        {
            . = ALIGN(4);
            *(.text)
            *(.text*)
            *(.rodata)
            *(.rodata*)
            . = ALIGN(4);
            _etext = .;
        } > FLASH

        .data : 
        {
            . = ALIGN(4);
            _sdata = .;
            *(.data)
            *(.data*)
            . = ALIGN(4);
            _edata = .;
        } > RAM AT> FLASH

        _sidata = LOADADDR(.data);

        .bss :
        {
            . = ALIGN(4);
            _sbss = .;
            *(.bss)
            *(.bss*)
            *(COMMON)
            . = ALIGN(4);
            _ebss = .;
        } > RAM

        .custom_buffer (NOLOAD):
        {
            . = ALIGN(4);
            _scustom_buffer = .;
            KEEP(*(.custom_buffer))
            . = ALIGN(4);
            _ecustom_buffer = .;
        } > BUFFER

        _sheap = .;
        _estack = ORIGIN(RAM) + LENGTH(RAM);
    }