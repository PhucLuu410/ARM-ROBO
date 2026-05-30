#include <fstream>
#include <iostream>
#include <windows.h>

int main()
{
    std::string filePath = "C:\\Users\\luuph\\OneDrive\\Desktop\\New folder\\Firmware_Application\\system\\build\\project.bin";
    std::ifstream binFile(filePath, std::ios::binary | std::ios::ate);
    if (!binFile.is_open())
    {
        std::cerr << "Loi: Khong tim thay file .bin!" << std::endl;
        return 1;
    }

    std::uint32_t fileLength = binFile.tellg();
    binFile.seekg(0, std::ios::beg);

    char lenHigh = (char)((fileLength >> 8) & 0xFF);
    char lenLow = (char)(fileLength & 0xFF);

    HANDLE hSerial = CreateFileA("\\\\.\\COM5", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hSerial == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Khong mo duoc COM5!" << std::endl;
        binFile.close();
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

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    SetCommTimeouts(hSerial, &timeouts);

    uint8_t byteNhan = 0;
    DWORD soByteDaNhan = 0;

    while (true)
    {
        byteNhan = 0;
        soByteDaNhan = 0;
        ReadFile(hSerial, &byteNhan, 1, &soByteDaNhan, NULL);
        if (soByteDaNhan > 0 && byteNhan == 0x28)
        {
            std::cout << "Nhan Tin Hieu" << std::endl;
            break;
        }
        Sleep(10);
    }

    DWORD soByteDaGui;

    std::cout << "Dang gui Co Update va Do dai file..." << std::endl;
    char header[] = {lenLow, lenHigh, 0};
    WriteFile(hSerial, header, 3, &soByteDaGui, NULL);

    std::cout << "Dang cho STM32 phan hoi 0x02..." << std::endl;
    byteNhan = 0;
    soByteDaNhan = 0;

    while (true)
    {
        byteNhan = 0;
        soByteDaNhan = 0;
        ReadFile(hSerial, &byteNhan, 1, &soByteDaNhan, NULL);
        if (soByteDaNhan > 0 && byteNhan == 0x02)
        {
            std::cout << "Bat dau truyen file..." << std::endl;
            break;
        }
        Sleep(10);
    }

    uint32_t tongSoByteDaTruyen = 0;
    while (true)
    {
        char buffer[1];
        binFile.read(buffer, 1);
        std::streamsize bytesReadFromFile = binFile.gcount();
        if (bytesReadFromFile == 0)
        {
            break;
        }

        WriteFile(hSerial, buffer, (DWORD)bytesReadFromFile, &soByteDaGui, NULL);

        tongSoByteDaTruyen += bytesReadFromFile;
        std::cout << "Da gui: " << tongSoByteDaTruyen << "/" << fileLength << " bytes. Cho 0x01..." << std::endl;

        while (true)
        {
            byteNhan = 0;
            soByteDaNhan = 0;
            ReadFile(hSerial, &byteNhan, 1, &soByteDaNhan, NULL);
            if (soByteDaNhan > 0 && byteNhan == 0x01)
            {
                break;
            }
            Sleep(1);
        }
    }

    byteNhan = 0;
    soByteDaNhan = 0;
    ReadFile(hSerial, &byteNhan, 1, &soByteDaNhan, NULL);
    if (soByteDaNhan > 0 && byteNhan == 0x04)
    {
        std::cout << "Da truyen tuyen tinh het file .bin!" << std::endl;
    }

    binFile.close();
    CloseHandle(hSerial);
    return 0;
}