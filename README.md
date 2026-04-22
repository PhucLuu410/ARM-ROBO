# 🤖 PROJECT ARM ROBOT (STM32 Bare-metal)

Dự án điều khiển cánh tay Robot dựa trên vi điều khiển **STM32F103 (Cortex-M3)**. Hệ thống được xây dựng hoàn toàn từ mức thanh ghi (Register-level), không sử dụng các thư viện trừu tượng như HAL hay LL, nhằm tối ưu hóa hiệu suất thời gian thực và quản lý bộ nhớ tuyệt đối.

---

## 🛠 Thông số Kỹ thuật (Technical Specs)
* **Core:** ARM Cortex-M3 (72MHz).
* **Methodology:** Bare-metal Programming (Startup & Linker Script Design).
* **Architecture:** Event-Driven kết hợp Finite State Machine (FSM).
* **Communication:** Modbus RTU Protocol (Custom Parser & CRC-16).
* **Peripherals:** TIM (PWM Control), UART (Interrupt-driven), ADC (Feedback), GPIO, EXTI.
* **Data Structures:** Ring Buffer (Circular Buffer) cho xử lý UART không nghẽn.

---

## 📂 Cấu trúc Thư mục (Project Tree)

```text
Robot_Project
├───app             # Logic ứng dụng (Modbus, PWM Control, Interrupts)
│   ├───inc         # Header files (.h)
│   └───src         # Source files (.c)
├───bsp             # Board Support Package (Driver: RCC, GPIO, UART, TIM...)
│   ├───inc         
│   └───src         
├───middle          # Lớp trung gian (Middleware)
├───DSA_utils       # Cấu trúc dữ liệu & Giải thuật (FSM, RingBuffer)
├───main            # Điểm khởi đầu chương trình (main.c/h)
└───system          # Cấu hình hệ thống cấp thấp (Low-level)
    ├───CMSIS       # Thư viện chuẩn lõi Cortex-M
    ├───linker      # Custom Linker Script (.ld)
    ├───startup     # Custom Startup Code (.c)
    └───build       # Chứa file thực thi (.bin, .elf) và Object files (.o)
