#include <fstream>
#include <iostream>
#include <windows.h>
#include <string>

int main()
{
    std::string filePath = "C:\\Users\\Phuc Luu\\OneDrive\\Desktop\\ARM-ROBO\\Firmware_Application\\system\\build\\project.bin";

    // 1. Mở cổng COM
    HANDLE hSerial = CreateFileA("\\\\.\\COM9", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hSerial == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Khong mo duoc COM5!" << std::endl;
        return 1;
    }

    // 2. Cấu hình Serial
    DCB dcb = {0};
    dcb.DCBlength = sizeof(dcb);
    GetCommState(hSerial, &dcb);
    dcb.BaudRate = CBR_9600; // Lưu ý: 9600 rất chậm, truyền file lớn sẽ lâu
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    SetCommState(hSerial, &dcb);

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 5000;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    SetCommTimeouts(hSerial, &timeouts);

    std::ifstream binFile(filePath, std::ios::binary);
    if (!binFile.is_open())
    {
        std::cerr << "Loi: Khong tim thay file!" << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    char buffer[16];
    DWORD bytesWritten, bytesRead;

    while (binFile.read(buffer, 16) || binFile.gcount() > 0)
    {
        WriteFile(hSerial, buffer, binFile.gcount(), &bytesWritten, NULL);
        std::cout << "Da gui " << bytesWritten << " byte. Dang cho ACK..." << std::endl;
        bool receivedAck = false;
        int timeoutCounter = 0;
        char ackBuffer[1];

        while (!receivedAck)
        {
            if (ReadFile(hSerial, ackBuffer, 1, &bytesRead, NULL))
            {
                if (bytesRead > 0 && ackBuffer[0] == 0x06)
                {
                    std::cout << "Nhan duoc ACK. Tiep tuc..." << std::endl;
                    receivedAck = true;
                }
            }
            else
            {
                std::cerr << "Loi ket noi Serial!" << std::endl;
                binFile.close();
                CloseHandle(hSerial);
                return -1;
            }

            Sleep(10);
            timeoutCounter++;
            if (timeoutCounter > 500)
            {
                std::cerr << "Timeout! STM32 khong phan hoi." << std::endl;
                binFile.close();
                CloseHandle(hSerial);
                return -1;
            }
        }
    }

    std::cout << "Da truyen xong file!" << std::endl;
    binFile.close();
    CloseHandle(hSerial);
    return 0;
}