#define APP_ADDR 0x08001000

#include <stdint.h>
#include "stm32f103xb.h"
#include <stddef.h>

typedef enum
{
    BOOT_IDLE_STATE,
    BOOT_WAIT_START_STATE,
    BOOT_SEND_START_STATE,
    BOOT_RECEIVE_STATE,
    BOOT_CHECK_DATA_STATE,
    BOOT_TRANSMIT_STATE,
} BootState;

volatile uint8_t currentByte = 0;
volatile uint16_t current_buffer = 0;
volatile uint8_t Update_Flag = 0;
volatile uint8_t count = 0;

uint8_t Data_Buffer[16] = {0};
uint8_t Start_Buffer[3] = {0};
volatile BootState bootState = BOOT_IDLE_STATE;
uint16_t *pLength = NULL;
uint32_t initialFileLength = 0;
volatile uint8_t Index = 0;
uint32_t current_address_write = 0;
void (*PtrApplication)(void);

static void delay(volatile uint32_t d)
{
    while (d--)
        ;
}

void System_Init(void)
{
    RCC->CR |= (1 << 0);
    while (!(RCC->CR & (1 << 1)))
        ;
    RCC->APB2ENR |= (1 << 0) | (1 << 2) | (1 << 4) | (1 << 9) | (1 << 14);

    GPIOA->CRL = 0x00000000;
    GPIOA->CRH = 0x00000000;
    GPIOA->CRL |= (1 << 16) | (1 << 17) | (1 << 18);
    GPIOA->CRH |= (0x0B << 4) | (8 << 8);
}

void Flash_Init(void)
{
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;
    while (FLASH->CR & FLASH_CR_LOCK)
        ;
}

void Flash_ErasePage(uint32_t Address)
{
    while (FLASH->SR & (1 << 0))
        ;
    FLASH->CR |= (1 << 1);
    FLASH->AR = Address;
    FLASH->CR |= (1 << 6);

    while (FLASH->SR & (1 << 0))
        ;
    FLASH->CR &= ~(1 << 1);
}

void Flash_WriteHalfWord(uint32_t Address, uint16_t Data)
{
    if (!(FLASH->SR & (1 << 0)))
    {
        FLASH->CR |= (1 << 0);
        *(volatile uint16_t *)Address = Data;
        while (FLASH->SR & (1 << 0))
            ;
        FLASH->CR &= ~(1 << 0);
    }
}

void Uart_Init(void)
{
    USART1->BRR = 0x341;
    USART1->CR1 |= (1 << 5) | (1 << 3) | (1 << 2);
    USART1->CR1 |= (1 << 13);
    NVIC_EnableIRQ(USART1_IRQn);
}

void Uart_SendData(uint8_t data)
{
    while (!(USART1->SR & (1 << 7)))
        ;
    USART1->DR = data;
}

void JumpToApp(void)
{
}

int main(void)
{
    System_Init();
    Flash_Init();
    Uart_Init();
    Index = 0;
    Update_Flag = 0;
    current_address_write = APP_ADDR;
    for (int i = 0; i < 16; i++)
    {
        Data_Buffer[i] = 0;
    }
    for (int i = 0; i < 60; i++)
    {
        Flash_ErasePage(APP_ADDR + (0x400 * i));
    }
    while (1)
    {
        if (Update_Flag == 1)
        {
            for (int i = 0; i < 8; i++)
            {
                uint16_t DataWrite = (Data_Buffer[(i * 2)]) | (Data_Buffer[(i * 2) + 1] << 8);
                Flash_WriteHalfWord(current_address_write, DataWrite);
                current_address_write = current_address_write + 0x02;
            }
            if (current_address_write == 0x08002900)
            {
                SCB->VTOR = 0x08001000;
                __set_MSP(*(volatile uint32_t *)0x08001000);
                uint32_t JumpAddress = *(volatile uint32_t *)(0x08001000 + 4);
                PtrApplication = (void (*)())JumpAddress;
                PtrApplication();
            }
            Update_Flag = 0;
            Uart_SendData(0x06);
        }
    }
}

void USART1_IRQHandler(void)
{
    while (!(USART1->SR & (1 << 5)))
        ;
    Data_Buffer[Index] = USART1->DR;
    Index++;
    if (Index == 16)
    {
        Update_Flag = 1;
        Index = 0;
    }
}

void HardFault_Handler(void)
{
    while (1)
    {
    }
}