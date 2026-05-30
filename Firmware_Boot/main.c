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
volatile uint32_t current_addr = APP_ADDR;
volatile uint8_t flag = 0;
volatile uint8_t count = 0;

uint8_t Data_Buffer[16] = {0};
uint8_t Start_Buffer[3] = {0};
volatile BootState bootState = BOOT_IDLE_STATE;
uint16_t *pLength = NULL;
uint32_t initialFileLength = 0;

static void delay(volatile uint32_t d)
{
    while (d--)
        ;
}

void Flash_Init(void)
{
    FLASH->ACR |= (1 << 4);
    while (!(FLASH->ACR & (1 << 5)))
        ;
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;
}

void Flash_ErasePage(uint32_t Address, uint32_t NumberPage)
{
    while (FLASH->SR & (1 << 0))
        ;
    FLASH->CR |= (1 << 1);
    for (int i = 0; i < NumberPage; i++)
    {
        FLASH->AR = Address + (i * 1024);
        FLASH->CR |= FLASH_CR_STRT;
        while (FLASH->SR & (1 << 0))
            ;
    }
    FLASH->CR &= ~(1 << 1);
}

void Flash_WriteHalfWord(uint32_t Address, uint16_t Data)
{
    while (FLASH->SR & (1 << 0))
        ;
    FLASH->CR |= (1 << 0);
    *(volatile uint16_t *)Address = Data;
    while (FLASH->SR & (1 << 0))
        ;
    FLASH->CR &= ~(1 << 0);
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

void JumpToApp(void)
{
    if (((*(volatile uint32_t *)APP_ADDR) & 0x2FFE0000) == 0x20000000)
    {
        uint32_t *App_Psp = (uint32_t *)APP_ADDR;
        uint32_t *App_Entry = (uint32_t *)(APP_ADDR + 4);

        __disable_irq();
        USART1->CR1 &= ~(1 << 13);

        __set_MSP(*App_Psp);
        void (*App_Jump)(void) = (void (*)(void))(*App_Entry);
        App_Jump();
    }
}

int main(void)
{
    System_Init();
    Flash_Init();
    Uart_Init();
    Flash_ErasePage(APP_ADDR, 60);

    currentByte = 0;
    bootState = BOOT_IDLE_STATE;
    count = 0;
    pLength = (uint16_t *)&Start_Buffer[1];

    while (1)
    {
        if (bootState == BOOT_IDLE_STATE)
        {
            Uart_SendData(0x28);
            bootState = BOOT_WAIT_START_STATE;
        }

        if (bootState == BOOT_SEND_START_STATE)
        {
            *pLength = ((*pLength & 0x00FF) << 8) | ((*pLength & 0xFF00) >> 8);
            initialFileLength = *pLength;

            Uart_SendData(0x02);
            bootState = BOOT_RECEIVE_STATE;
        }

        if (bootState == BOOT_CHECK_DATA_STATE)
        {
            uint8_t halfWordsToWrite = (currentByte + 1) / 2;

            for (int i = 0; i < halfWordsToWrite; i++)
            {
                uint16_t Data = (Data_Buffer[2 * i + 1] << 8) | Data_Buffer[2 * i];
                Flash_WriteHalfWord(current_addr, Data);
                current_addr += 2;
            }

            currentByte = 0;

            if (*pLength == 0)
            {
                Uart_SendData(0x04);
                delay(100);
                JumpToApp();
            }
            else
            {
                bootState = BOOT_RECEIVE_STATE;
                Uart_SendData(0x01);
            }
        }
    }
}

void USART1_IRQHandler(void)
{
    if (USART1->SR & (1 << 5))
    {
        uint8_t temp_rx = (uint8_t)USART1->DR;

        if (bootState == BOOT_WAIT_START_STATE)
        {
            Start_Buffer[count++] = temp_rx;
            if (count == 3)
            {
                bootState = BOOT_SEND_START_STATE;
                count = 0;
            }
        }

        else if (bootState == BOOT_RECEIVE_STATE && *pLength > 0)
        {
            Data_Buffer[currentByte++] = temp_rx;
            (*pLength)--;

            if (currentByte == 16 || *pLength == 0)
            {
                bootState = BOOT_CHECK_DATA_STATE;
            }
            else
            {
                Uart_SendData(0x01);
            }
        }
    }
}