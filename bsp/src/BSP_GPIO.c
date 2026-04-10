#include "BSP_GPIO.h"

SystemFlag System_GPIO_Flag;

void GPIO_Config(GPIO_TypeDef *GPIOx, uint8_t pin, uint8_t mode, uint8_t cnf)
{
    volatile uint32_t *reg;

    uint8_t pos;

    if (pin < 8)
    {
        reg = &GPIOx->CRL;
        pos = pin * 4;
    }
    else
    {
        reg = &GPIOx->CRH;
        pos = (pin - 8) * 4;
    }

    *reg &= ~(0xF << pos);

    *reg |= ((mode | (cnf << 2)) << pos);
}

void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint8_t pin, uint8_t state)
{
    if (state)
        GPIOx->BSRR = (1 << pin);
    else
        GPIOx->BRR = (1 << pin);
}

void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint8_t pin)
{
    GPIOx->ODR ^= (1 << pin);
}

void GPIO_Test_Polling_Live(void)
{
    GPIO_Config(GPIOC, 13, 0x2, 0x0);

    GPIO_TogglePin(GPIOC, 13);
    for (int i = 0; i < 1000; i++)
        ;
}

void EXTI15_10_IRQHandler(void)
{
    if (EXTI->PR & (1 << 11))
    {
        EXTI->PR |= (1 << 11);
        System_GPIO_Flag.BUTTON_FLAG = 1;
    }

    if (EXTI->PR & (1 << 12))
    {
        EXTI->PR |= (1 << 12);
        System_GPIO_Flag.EMERGENCY_FLAG = 1;
    }
}