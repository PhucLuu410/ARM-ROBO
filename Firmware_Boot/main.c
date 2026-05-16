#define APP_ADDR 0x08001000

#include <stdint.h>
#include "stm32f103xb.h"

static void delay(volatile uint32_t d)
{
    while (d--)
        ;
}

typedef void (*AppEntry)(void);

void Clock_Init(void)
{
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY))
        ;
}
void UART_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    GPIOA->CRH &= ~(0xF << 4);
    GPIOA->CRH |= (0xB << 4);
    GPIOA->CRH &= ~(0xF << 8);
    GPIOA->CRH |= (0x4 << 8);
    USART1->BRR = 0x341;
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void UART_WriteByte(uint8_t data)
{
    while (!(USART1->SR & USART_SR_TXE))
        ;

    USART1->DR = data;
}

uint8_t UART_ReadByteTimeout(uint32_t timeout)
{
    while (timeout--)
    {
        if (USART1->SR & USART_SR_RXNE)
        {
            return USART1->DR;
        }
    }
    return 0;
}

void jump_to_app(void)
{
    uint32_t app_sp = *(volatile uint32_t *)APP_ADDR;
    uint32_t app_reset = *(volatile uint32_t *)(APP_ADDR + 4);
    if ((app_sp & 0x2FFE0000) != 0x20000000)
        return;
    __disable_irq();
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;
    NVIC->ICER[0] = 0xFFFFFFFF;
    NVIC->ICPR[0] = 0xFFFFFFFF;
    SCB->VTOR = APP_ADDR;
    __set_MSP(app_sp);
    AppEntry app = (AppEntry)(app_reset | 1U);
    app();
}

void Flash_Unlock(void)
{
    if (FLASH->CR & FLASH_CR_LOCK)
    {
        FLASH->KEYR = 0x45670123;
        FLASH->KEYR = 0xCDEF89AB;
    }
}

void Flash_ErasePage(uint32_t address)
{
    while (FLASH->SR & FLASH_SR_BSY)
        ;

    FLASH->CR |= FLASH_CR_PER;

    FLASH->AR = address;

    FLASH->CR |= FLASH_CR_STRT;

    while (FLASH->SR & FLASH_SR_BSY)
        ;

    FLASH->CR &= ~FLASH_CR_PER;
}

void Flash_WriteHalfWord(uint32_t address, uint16_t data)
{
    while (FLASH->SR & FLASH_SR_BSY)
        ;

    FLASH->CR |= FLASH_CR_PG;

    *(volatile uint16_t *)address = data;

    while (FLASH->SR & FLASH_SR_BSY)
        ;

    FLASH->CR &= ~FLASH_CR_PG;
}

void Boot_ReceiveFirmware(void)
{
    uint32_t size = 0;
    uint8_t *p = (uint8_t *)&size;
    for (int i = 0; i < 4; i++)
    {
        p[i] = UART_ReadByteTimeout(0xFFFFFF);
    }
    Flash_Unlock();
    for (uint32_t addr = APP_ADDR;
         addr < (APP_ADDR + size + 1024);
         addr += 1024)
    {
        Flash_ErasePage(addr);
    }
    uint32_t flash_addr = APP_ADDR;
    for (uint32_t i = 0; i < size; i += 2)
    {
        uint16_t data = 0xFFFF;

        uint8_t low = UART_ReadByteTimeout(0xFFFFFF);
        uint8_t high = 0xFF;
        if ((i + 1) < size)
        {
            high = UART_ReadByteTimeout(0xFFFFFF);
        }
        data = low | (high << 8);
        Flash_WriteHalfWord(flash_addr, data);
        flash_addr += 2;
    }
    for (int i = 0; i < 3; i++)
    {
        GPIOC->ODR ^= (1 << 13);
        delay(500000);
    }
    jump_to_app();
}

int main(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    GPIOC->CRH &= ~(0xF << 20);
    GPIOC->CRH |= (0x2 << 20);
    volatile uint8_t Update_Flag = 0;
    GPIOC->ODR |= (1 << 13);
    Clock_Init();
    UART_Init();
    UART_WriteByte('U');
    uint8_t resp = UART_ReadByteTimeout(3000000);
    if (resp == 'Y')
    {
        GPIOC->ODR &= ~(1 << 13);
        delay(500000);
        Update_Flag = 1;
    }
    if (Update_Flag == 0)
    {
        for (int i = 0; i < 3; i++)
        {
            GPIOC->ODR ^= (1 << 13);
            delay(500000);
        }
        jump_to_app();
    }
    while (1)
    {
        if (Update_Flag == 1)
        {
            UART_WriteByte('S');
            Boot_ReceiveFirmware();
        }
    }
}