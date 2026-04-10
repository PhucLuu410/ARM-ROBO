#ifndef __MODBUS_H
#define __MODBUS_H

#include "DSA.h"
#include "BSP.h"
#include <string.h>
#include <stdint.h>

typedef struct
{
    uint8_t slave_addr;
    uint8_t func_code;
    uint16_t servo_1;
    uint16_t servo_2;
    uint16_t servo_3;
    uint16_t servo_4;
    uint16_t crc;
} ModbusRequest;

void MODBUS_Parse_Request(void);
bool Get_MODBUS_Data(uint8_t *data);
#endif