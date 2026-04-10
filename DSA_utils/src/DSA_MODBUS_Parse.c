#include "DSA_MODBUS_Parse.h"

#define BUFFER_SIZE 256
#define MODBUS_FRAME_SIZE 12

extern volatile int front;
extern volatile int rear;
extern volatile uint8_t buffer[BUFFER_SIZE];

uint16_t CRC_Check = 0;
extern SystemFlag Control_Robot_Flag;
uint8_t request_buffer[12];

__attribute__((section(".custom_buffer"))) ModbusRequest request;

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
    if (Control_Robot_Flag.MODBUS_FLAG == 1)
    {
        Get_MODBUS_Data(request_buffer);
        uint16_t CRC_Check = MODBUS_CRC_Check(request_buffer, 10);
        if (CRC_Check != ((request_buffer[10] << 8) | request_buffer[11]))
        {
            Control_Robot_Flag.MODBUS_FLAG = 0;
            return;
        }
        request.slave_addr = request_buffer[0];
        request.func_code = request_buffer[1];
        request.servo_1 = (request_buffer[2] << 8) | request_buffer[3];
        request.servo_2 = (request_buffer[4] << 8) | request_buffer[5];
        request.servo_3 = (request_buffer[6] << 8) | request_buffer[7];
        request.servo_4 = (request_buffer[8] << 8) | request_buffer[9];
        request.crc = (request_buffer[10] << 8) | request_buffer[11];
        Control_Robot_Flag.MODBUS_FLAG = 0;
    }
}

bool Get_MODBUS_Data(uint8_t *data)
{
    if (RingBuffer_Available(&front, &rear, BUFFER_SIZE) != MODBUS_FRAME_SIZE)
    {
        front = rear;
        return false;
    }
    for (int i = 0; i < MODBUS_FRAME_SIZE; i++)
    {
        data[i] = RingBuffer_Pop(&front, &rear, buffer, BUFFER_SIZE);
    }
    return true;
}