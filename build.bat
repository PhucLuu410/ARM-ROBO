@echo off
setlocal enabledelayedexpansion

REM ===== Cấu hình trình biên dịch =====
set CC=arm-none-eabi-gcc
set OBJCOPY=arm-none-eabi-objcopy

REM ===== Flags (THÊM -DSTM32F103xB Ở ĐÂY) =====
set CFLAGS=-c -mcpu=cortex-m3 -mthumb -g -O0 -DSTM32F103xB 
set LDFLAGS=-T system/linker/STM32F103C8T6_Linker.ld -g -nostdlib
set INCLUDES=-I./system/CMSIS -I./bsp/inc -I./main/inc -I./app/inc -I./DSA_utils/inc -I./library/inc

REM ===== Thư mục đầu ra =====
set BUILD_DIR=system/build
if not exist %BUILD_DIR% mkdir %BUILD_DIR%

REM ===== 1. Biên dịch các file .c =====
set OBJECTS=
for /r %%f in (*.c) do (
    set "SRC_FILE=%%f"
    set "OBJ_FILE=!SRC_FILE:%CD%=!"
    set "OBJ_FILE=!OBJ_FILE:.c=.o!"
    set "OBJ_FILE=%BUILD_DIR%!OBJ_FILE!"
    rem Chuyển / thành \ cho Windows
    set "OBJ_FILE=!OBJ_FILE:/=\!"
    rem Tạo thư mục chứa file .o nếu chưa có
    for %%d in ("!OBJ_FILE!") do if not exist "%%~dpd" mkdir "%%~dpd"
    echo Compiling: %%f
    %CC% %CFLAGS% %INCLUDES% "%%f" -o "!OBJ_FILE!"
    if !errorlevel! neq 0 (
        echo ---------------------------------------
        echo ERROR in: %%f
        pause
        exit /b !errorlevel!
    )
    set OBJECTS=!OBJECTS! "!OBJ_FILE!"
)

REM ===== 2. Link các file .o =====
echo ---------------------------------------
echo Linking into %BUILD_DIR%\project.elf...
%CC% %LDFLAGS% !OBJECTS! -o %BUILD_DIR%/project.elf

if %errorlevel% equ 0 (
    %OBJCOPY% -O binary %BUILD_DIR%/project.elf %BUILD_DIR%/project.bin
    echo ---------------------------------------
    echo BUILD SUCCESSFUL!
    echo Output: %BUILD_DIR%\project.elf
) else (
    echo ---------------------------------------
    echo LINKING FAILED!
)