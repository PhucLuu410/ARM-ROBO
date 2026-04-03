#include "APP_MODBUS.h"

uint8_t request_buffer[8];
ModbusRequest request;
extern SystemFlag main_flag;
uint16_t CRC_Check = 0;

uint16_t MODBUS_CRC_Check(uint8_t *buf, int len)
{
    uint16_t crc = 0xFFFF;

    for (int i = 0; i < len; i++)
    {
        crc ^= buf[i];

        for (int j = 0; j < 8; j++)
        {
            if (crc & 0x0001)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc >>= 1;
        }
    }

    return crc;
}

void MODBUS_Parse_Request(void)
{
    if (main_flag.MODBUS_FLAG == 1)
    {
        Get_MODBUS_Data(request_buffer);
        if (MODBUS_CRC_Check(request_buffer, 6) != ((request_buffer[6] << 8) | request_buffer[7]))
        {
            main_flag.MODBUS_FLAG = 0;
            return;
        }
        request.slave_addr = request_buffer[0];
        request.func_code = request_buffer[1];
        request.start_addr = (request_buffer[2] << 8) | request_buffer[3];
        request.reg_num = (request_buffer[4] << 8) | request_buffer[5];
        request.crc = (request_buffer[6] << 8) | request_buffer[7];
        main_flag.MODBUS_FLAG = 0;
    }
}
