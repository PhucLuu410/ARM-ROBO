#define APP_ADDR 0x08001000

#include <stdint.h>
#include "stm32f103xb.h"

static void delay(volatile uint32_t d)
{
    while (d--)
        ;
}

typedef void (*AppEntry)(void);

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

int main(void)
{
    /* test LED boot */
    RCC->APB2ENR |= (1 << 4); // GPIOC clock

    GPIOC->CRH &= ~(0xF << 20);
    GPIOC->CRH |= (0x2 << 20);
    GPIOC->ODR ^= (1 << 13);
    jump_to_app();

    while (1)
    {
        GPIOC->ODR ^= (1 << 13);
        delay(500000);
    }
}