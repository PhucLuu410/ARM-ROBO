#include <windows.h>
#include <iostream>
#include <stdint.h>

HANDLE hSerial;

// ================= CRC MODBUS =================
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

// ================= SEND DATA =================
void SendData(const unsigned char *data, int length)
{
    DWORD bytesWritten;
    WriteFile(hSerial, data, length, &bytesWritten, NULL);
}

// ================= SEND PWM FRAME =================
void SendPWM(int pwm1, int pwm2, int pwm3 = 0, int pwm4 = 0)
{
    uint8_t frame[12];

    frame[0] = 0x01;
    frame[1] = 0x01;

    frame[2] = (pwm1 >> 8) & 0xFF;
    frame[3] = pwm1 & 0xFF;

    frame[4] = (pwm2 >> 8) & 0xFF;
    frame[5] = pwm2 & 0xFF;

    frame[6] = (pwm3 >> 8) & 0xFF;
    frame[7] = pwm3 & 0xFF;

    frame[8] = (pwm4 >> 8) & 0xFF;
    frame[9] = pwm4 & 0xFF;

    uint16_t crc = MODBUS_CRC_Check(frame, 10);
    frame[10] = (crc >> 8) & 0xFF;
    frame[11] = crc & 0xFF;

    SendData(frame, 12);
}

// ================= PWM CONFIG =================
const int PWM_1_MIN = 1250;
const int PWM_1_MAX = 1500;
const int PWM_2_3_MIN = 600;
const int PWM_2_3_MAX = 1800;
const int PWM_4_MIN = 700;
const int PWM_4_MAX = 2500;

// ================= WINDOW PROC =================
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static int pwm1 = 1000;
    static int pwm2 = 1000;
    static int pwm3 = 1000;
    static int pwm4 = 1000;

    static int pwm1_smooth = 1200;
    static int pwm2_smooth = 1200;
    static int pwm3_smooth = 1200;
    static int pwm4_smooth = 1200;

    static DWORD lastTime = 0;

    switch (uMsg)
    {

        // ================= BUTTON TOGGLE =================

    case WM_LBUTTONDOWN:
    {
        pwm1_smooth = 1300; // đè chuột
        SendPWM(pwm1_smooth, pwm2_smooth, pwm3_smooth, pwm4_smooth);
        break;
    }

    case WM_LBUTTONUP:
    {
        pwm1_smooth = 1500; // thả chuột
        SendPWM(pwm1_smooth, pwm2_smooth, pwm3_smooth, pwm4_smooth);
        break;
    }

    // ================= MOUSE MOVE =================
    case WM_MOUSEMOVE:
    {
        int mouseX = LOWORD(lParam);
        int mouseY = HIWORD(lParam);

        RECT rect;
        GetClientRect(hwnd, &rect);

        int width = rect.right;
        int height = rect.bottom;

        pwm2 = PWM_2_3_MIN + ((width - 1 - mouseX) * (((PWM_2_3_MAX - PWM_2_3_MIN)) / (width - 1)));
        pwm3 = PWM_2_3_MIN + ((height - 1 - mouseY) * (((PWM_2_3_MAX - PWM_2_3_MIN)) / (height - 1)));

        if (pwm2 < PWM_2_3_MIN)
            pwm2 = PWM_2_3_MIN;
        if (pwm2 > PWM_2_3_MAX)
            pwm2 = PWM_2_3_MAX;

        if (pwm3 < PWM_2_3_MIN)
            pwm3 = PWM_2_3_MIN;
        if (pwm3 > PWM_2_3_MAX)
            pwm3 = PWM_2_3_MAX;

        pwm2_smooth += (pwm2 - pwm2_smooth) * 0.2;
        pwm3_smooth += (pwm3 - pwm3_smooth) * 0.2;
        DWORD now = GetTickCount();
        if (now - lastTime > 40)
        {
            SendPWM(pwm1_smooth, pwm2_smooth, pwm3_smooth, pwm4_smooth);
            lastTime = now;
        }

        break;
    }

        // ================= MOUSE WHEEL =================

    case WM_MOUSEWHEEL:
    {
        int delta = GET_WHEEL_DELTA_WPARAM(wParam);

        if (delta > 0)
        {
            if (pwm4 < PWM_4_MAX)
                pwm4 += 10;
        }
        else
        {
            if (pwm4 > PWM_4_MIN)
                pwm4 -= 10;
        }

        // clamp
        if (pwm4 < PWM_4_MIN)
            pwm4 = PWM_4_MIN;
        if (pwm4 > PWM_4_MAX)
            pwm4 = PWM_4_MAX;

        pwm4_smooth += (pwm4 - pwm4_smooth) * 0.2;
        DWORD now = GetTickCount();
        if (now - lastTime > 40)
        {
            SendPWM(pwm1_smooth, pwm2_smooth, pwm3_smooth, pwm4_smooth);
            lastTime = now;
        }

        break;
    }

    case WM_DESTROY:
        CloseHandle(hSerial);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// ================= MAIN =================
int main()
{
    hSerial = CreateFile("COM5", GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);

    if (hSerial == INVALID_HANDLE_VALUE)
    {
        MessageBox(NULL, "Cannot open COM5", "Error", MB_OK);
        return 1;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    GetCommState(hSerial, &dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    SetCommState(hSerial, &dcbSerialParams);

    const char CLASS_NAME[] = "ServoControl";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Robot Arm Mouse Control",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        NULL,
        NULL,
        NULL,
        NULL);

    ShowWindow(hwnd, SW_SHOW);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}