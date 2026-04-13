#include "BSP_SPI.h"

void SPI_Init(void)
{
    GPIO_Config(GPIOA, 15, GPIO_MODE_OUTPUT_50M, GPIO_CNF_GP_PP); // CSS
    GPIO_Config(GPIOB, 3, GPIO_MODE_OUTPUT_50M, GPIO_CNF_AF_PP);  // SCK
    GPIO_Config(GPIOB, 4, GPIO_MODE_INPUT, GPIO_CNF_GP_OD);       // MISO
    GPIO_Config(GPIOB, 5, GPIO_MODE_OUTPUT_50M, GPIO_CNF_AF_PP);  // MOSI
    AFIO->MAPR |= (1 << 0);                                       // SPI1 remap

    SPI1->CR1 = (1 << 2) | (1 << 1) | (1 << 0); // Master mode, SSM, SSI
    SPI1->CR1 |= (7 << 3);                      // Baud rate = fPCLK/256
    SPI1->CR1 |= (1 << 6);                      // Enable SPI
}

void SPI_SendByte(uint8_t data)
{
    while (!(SPI1->SR & (1 << 1)))
        ;            // Wait until TXE is set
    SPI1->DR = data; // Write data to be transmitted
}
