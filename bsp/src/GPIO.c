#include "GPIO.h"

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