#include "modbus.h"

void MODBUS_Parse_Request(uint8_t *data, ModbusRequest *request)
{
    request->slave_addr = data[0];
    request->func_code = data[1];
    request->start_addr = (data[2] << 8) | data[3];
    request->reg_num = (data[4] << 8) | data[5];
    request->crc = (data[6] << 8) | data[7];
}

uint16_t MODBUS_CRC_Check(ModbusRequest *request)
{
    uint16_t check = 0xFFFF;
    const uint8_t *ptr = (uint8_t *)request;

    for (int i = 0; i < 6; i++)
    {
        check = check ^ ptr[i];

        for (int j = 0; j < 8; j++)
        {
            if (check & 0x0001)
            {
                check = (check >> 1) ^ 0xA001;
            }
            else
            {
                check = check >> 1;
            }
        }
    }

    request->crc = check;
    return check;
}
