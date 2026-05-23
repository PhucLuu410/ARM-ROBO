#include <fstream>
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>

int main()
{
    std::string filePath = "C:\\Users\\luuph\\OneDrive\\Desktop\\New folder\\Firmware_Application\\system\\build\\project.bin";

    // 1. Mở cổng COM
    HANDLE hSerial = CreateFileA("\\\\.\\COM5", GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
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

    std::cout << "Bat dau truyen du lieu..." << std::endl;
    DWORD bytesWritten;
    char buffer[16];

    while (binFile.read(buffer, 16) || binFile.gcount() > 0)
    {
        WriteFile(hSerial, buffer, binFile.gcount(), &bytesWritten, NULL);
        Sleep(30);
    }

    std::cout << "Da truyen xong file!" << std::endl;

    binFile.close();
    CloseHandle(hSerial);
    return 0;
}