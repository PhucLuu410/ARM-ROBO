PROJECT ARM ROBOT

TECH.
Bare metal STM32F1 Cortex M3 TIM, UART(MODBUS RTU), ADC, GPIO, EXTI, AFIO
Design Linker Script and StartUp
Bootloader
Event Driven and Finite System State

Project Tree:
Robot_Project 
├───app
│   ├───inc
│   │       app.h
│   │       change_pwm_duty.h
│   │       interupt.h
│   │       modbus.h
│   │
│   └───src
│           change_pwm_duty.c
│           interupt.c
│           modbus.c
│
├───bsp
│   ├───inc
│   │       bsp.h
│   │       EXTI.h
│   │       GPIO.h
│   │       RCC.h
│   │       TIM.h
│   │       UART.h
│   │
│   └───src
│           EXTI.c
│           GPIO.c
│           RCC.c
│           TIM.c
│           UART.c
│
├───DSA_utils
│   ├───inc
│   │       FSM.h
│   │       RingBuffer.h
│   │       utils.h
│   │
│   └───src
│           FSM.c
│           RingBuffer.c
│
├───main
│   ├───inc
│   │       main.h
│   │
│   └───src
│           main.c
│
├───middle
│   ├───inc
│   └───src
└───system
    ├───build
    │   │   project.bin
    │   │   project.elf
    │   │
    │   ├───app
    │   │   └───src
    │   │           change_pwm_duty.o
    │   │           interupt.o
    │   │           modbus.o
    │   │           mosbus.o
    │   │
    │   ├───bsp
    │   │   └───src
    │   │           EXTI.o
    │   │           GPIO.o
    │   │           RCC.o
    │   │           TIM.o
    │   │           UART.o
    │   │
    │   ├───DSA_utils
    │   │   │   RingBuffer.o
    │   │   │
    │   │   └───src
    │   │           FSM.o
    │   │           RingBuffer.o
    │   │
    │   ├───main
    │   │   └───src
    │   │           main.o
    │   │
    │   └───system
    │       ├───CMSIS
    │       │       system_stm32f1xx.o
    │       │
    │       ├───src
    │       │       syscalls.o
    │       │
    │       └───startup
    │               STM32F103C8T6_StartUp.o
    │
    ├───CMSIS
    │       cmsis_compiler.h
    │       cmsis_gcc.h
    │       cmsis_version.h
    │       core_cm3.h
    │       stm32f103xb.h
    │       stm32f1xx.h
    │       system_stm32f1xx.c
    │       system_stm32f1xx.h
    │       
    ├───debug
    │       STM32F103.svd
    │
    ├───linker
    │       STM32F103C8T6_Linker.ld
    │
    ├───src
    │       syscalls.c
    │
    └───startup
            STM32F103C8T6_StartUp.c



app: Application Function.
bsp: Board Packet Support.
middle: MiddleWare.
System: For Cortex M, linker script, startup code, build file, CMSIS lib.
DSA-utils: DSA using for this project