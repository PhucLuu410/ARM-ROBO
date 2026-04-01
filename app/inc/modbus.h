#ifndef __MODBUS_H
#define __MODBUS_H

#include "RingBuffer.h"
#include <string.h>
#include <stdint.h>
typedef struct
{
    uint8_t slave_addr;
    uint8_t func_code;
    uint16_t start_addr;
    uint16_t reg_num;
    uint16_t crc;
} ModbusRequest;

void MODBUS_Parse_Request(uint8_t *data, ModbusRequest *request);
uint16_t MODBUS_CRC_Check(ModbusRequest *request);

#endif