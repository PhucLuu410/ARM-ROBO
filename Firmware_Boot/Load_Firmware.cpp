#include <fstream>
#include <iostream>
#include <windows.h>
#include <string>

typedef enum
{
    FLAG_INIT = 0,
    FLAG_READY,
    FLAG_START,
    FLAG_SEND,
    FLAG_WAIT,
    FLAG_DONE,
} UpdateFlagType;

UpdateFlagType UpdateFlag = FLAG_INIT;

int main()
{
    std::string filePath = "C:\\Users\\luuph\\OneDrive\\Desktop\\ROBOT_ARM\\Firmware_Application\\system\\build\\project.bin";

    HANDLE hSerial = CreateFileA("\\\\.\\COM5", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hSerial == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Khong mo duoc COM5!" << std::endl;
        return 1;
    }

    DCB dcb = {0};
    dcb.DCBlength = sizeof(dcb);
    GetCommState(hSerial, &dcb);
    dcb.BaudRate = CBR_9600;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    SetCommState(hSerial, &dcb);

    std::ifstream binFile(filePath, std::ios::binary);
    if (!binFile.is_open())
    {
        std::cerr << "Loi: Khong tim thay file!" << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    binFile.seekg(0, std::ios::end);
    std::streamsize fileSize = binFile.tellg();
    binFile.seekg(0, std::ios::beg);

    std::cout << "Mo file thanh cong!" << std::endl;
    std::cout << "Dung luong file: " << fileSize << " bytes" << std::endl;

    char Index = 0;
    std::string buffer;
    char rxBuffer[16];
    DWORD bytesWritten, bytesRead;

    while (true)
    {
        if (ReadFile(hSerial, &Index, 1, &bytesRead, NULL) && bytesRead > 0)
        {
            buffer += Index;
        }
        if (buffer.length() >= 2)
        {
            unsigned char b0 = (unsigned char)buffer[0];
            unsigned char b1 = (unsigned char)buffer[1];

            if (b0 == 0x10 && b1 == 0x01 && UpdateFlag == FLAG_INIT)
            {
                std::cout << "Nhan duoc lenh INIT. Dang gui metadata (4 bytes)..." << std::endl;

                uint32_t sizeToSend = static_cast<uint32_t>(fileSize);

                uint8_t sizeBytes[16];

                sizeBytes[0] = (sizeToSend & 0xFF);
                sizeBytes[1] = ((sizeToSend >> 8) & 0xFF);
                sizeBytes[2] = ((sizeToSend >> 16) & 0xFF);
                sizeBytes[3] = ((sizeToSend >> 24) & 0xFF);
                sizeBytes[4] = 0x00;  // Byte 5
                sizeBytes[5] = 0x00;  // Byte 6
                sizeBytes[6] = 0x00;  // Byte 7
                sizeBytes[7] = 0x00;  // Byte 8
                sizeBytes[8] = 0x00;  // Byte 5
                sizeBytes[9] = 0x00;  // Byte 6
                sizeBytes[10] = 0x00; // Byte 7
                sizeBytes[11] = 0x00; // Byte 8
                sizeBytes[12] = 0x00; // Byte 5
                sizeBytes[13] = 0x00; // Byte 6
                sizeBytes[14] = 0x00; // Byte 7
                sizeBytes[15] = 0x00; // Byte 8

                if (WriteFile(hSerial, sizeBytes, sizeof(sizeBytes), &bytesWritten, NULL))
                {
                    std::cout << "Da gui metadata: " << sizeToSend << " bytes (duoi dang mang 16 byte)." << std::endl;
                }

                UpdateFlag = FLAG_READY;
                buffer.clear();
            }

            else if (b0 == 0x10 && b1 == 0x03 && UpdateFlag == FLAG_READY)
            {
                std::cout << "Nhan duoc lenh start" << std::endl;
                UpdateFlag = FLAG_START;

                binFile.read(rxBuffer, 16);
                std::streamsize bytesReadFromFile = binFile.gcount();

                WriteFile(hSerial, rxBuffer, bytesReadFromFile, &bytesWritten, NULL);
                std::cout << "Da gui 16 byte dau tien." << std::endl;

                buffer.clear();
                UpdateFlag = FLAG_WAIT;
            }

            else if (b0 == 0x30 && b1 == 0x00 && UpdateFlag == FLAG_WAIT)
            {
                std::cout << "Nhan duoc lenh continue" << std::endl;
                UpdateFlag = FLAG_SEND;
                buffer.clear();
            }
            else
            {
                buffer.erase(0, 1);
            }
        }

        if (UpdateFlag == FLAG_SEND)
        {
            binFile.read(rxBuffer, 16);
            std::streamsize bytesReadFromFile = binFile.gcount();

            if (bytesReadFromFile == 0)
            {
                std::cout << "=== DA GUI HET FILE BIN ===" << std::endl;
                break;
            }

            WriteFile(hSerial, rxBuffer, bytesReadFromFile, &bytesWritten, NULL);
            std::cout << "Da gui tiep " << bytesWritten << " byte." << std::endl;

            UpdateFlag = FLAG_WAIT;
        }

        Sleep(1);
    }

    std::cout << "Ket thuc" << std::endl;
    binFile.close();
    CloseHandle(hSerial);
    return 0;
}