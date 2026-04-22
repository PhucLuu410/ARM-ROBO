#include "BSP_SPI.h"

void SPI_Init(void)
{
    GPIO_Config(GPIOA, 15, GPIO_MODE_OUTPUT_50M, GPIO_CNF_GP_PP); // CSS
    GPIO_WritePin(GPIOA, 15, 0);
    GPIO_Config(GPIOB, 3, GPIO_MODE_OUTPUT_50M, GPIO_CNF_AF_PP);  // SCK
    GPIO_Config(GPIOB, 4, GPIO_MODE_INPUT, GPIO_CNF_GP_OD);       // MISO
    GPIO_Config(GPIOB, 5, GPIO_MODE_OUTPUT_50M, GPIO_CNF_AF_PP);  // MOSI
    AFIO->MAPR |= (1 << 0);                                       // SPI1 remap
    AFIO->MAPR |= (2 << 24);                                      // disable JTAG, giữ SWD
    SPI1->CR1 &= ~(1 << 11);                                      // 8 Bit
    SPI1->CR1 |= (1 << 9);                                        // SSM = 1 (software NSS)
    SPI1->CR1 |= (1 << 8);                                        // SSI = 1
    // SPI1->CR2 |= (1 << 7);                      // TX Interrupt enable
    SPI1->CR1 |= (7 << 3);                      // Baud rate = fPCLK/256
    SPI1->CR1 |= (1 << 2);                      // Master mode
    SPI1->CR1 &= ~(1 << 1);                     // CPOL = 0
    SPI1->CR1 &= ~(1 << 0);                     // CPHA = 0
    SPI1->CR1 |= (1 << 6);                      // Enable SPI
}

void SPI_SendByte(uint8_t data)
{
    while (!(SPI1->SR & (1 << 1)))
        ; // TXE

    SPI1->DR = data;

    while (!(SPI1->SR & (1 << 0)))
        ; // RXNE

    uint8_t temp = SPI1->DR;
}

void SPI_SendData(uint8_t *data, uint16_t size)
{
    GPIO_WritePin(GPIOA, 15, 0); // CS LOW

    for (uint16_t i = 0; i < size; i++)
    {
        SPI_SendByte(data[i]);
    }

    while (SPI1->SR & (1 << 7))
        ; // BSY clear

    GPIO_WritePin(GPIOA, 15, 1); // CS HIGH
}
