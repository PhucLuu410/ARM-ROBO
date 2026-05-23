#define APP_ADDR 0x08001000

#include <stdint.h>
#include "stm32f103xb.h"

volatile uint8_t currentByte = 0;
volatile uint16_t current_buffer = 0;
volatile uint32_t current_addr = APP_ADDR;
volatile uint8_t flag = 0;
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

void Flash_MassErase(void)
{
    while (FLASH->SR & FLASH_SR_BSY)
        ;
    FLASH->CR |= FLASH_CR_MER;
    FLASH->CR |= FLASH_CR_STRT;
    while (FLASH->SR & FLASH_SR_BSY)
        ;
    FLASH->CR &= ~FLASH_CR_MER;
}

void Uart_Init(void)
{
    USART1->BRR = 0x341;
    USART1->CR1 |= (1 << 5) | (1 << 3) | (1 << 2);
    USART1->CR1 |= (1 << 13);
    NVIC_EnableIRQ(USART1_IRQn);
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
    uint32_t *App_Psp = (uint32_t *)APP_ADDR;
    uint32_t *App_Entry = (uint32_t *)(APP_ADDR + 4);
    __set_MSP(*App_Psp);
    void (*App_Jump)(void) = (void (*)(void))(*App_Entry);
    App_Jump();
}

int main(void)
{
    System_Init();
    Flash_Init();
    Uart_Init();
    Flash_ErasePage(APP_ADDR, 60);
    currentByte = 0;
    // Flash_MassErase();
    // JumpToApp();
    while (1)
    {
    }
}

void USART1_IRQHandler(void)
{
    if (USART1->SR & (1 << 5))
    {
        volatile uint8_t data = (uint8_t)USART1->DR;
        if (currentByte == 0)
        {
            current_buffer = data & 0x00FF;
            currentByte = 1;
        }
        if (currentByte == 1)
        {
            current_buffer |= (data << 8);
            Flash_WriteHalfWord(current_addr, current_buffer);
            current_addr += 2;
            currentByte = 0;
        }
    }
}