@echo off
setlocal enabledelayedexpansion

REM ===== Toolchain =====
set CC=arm-none-eabi-gcc
set OBJCOPY=arm-none-eabi-objcopy

REM ===== OpenOCD =====
set OPENOCD=openocd

REM ===== Flags =====
set CFLAGS=-c -mcpu=cortex-m3 -mthumb -g -O0 -DSTM32F103xB
set LDFLAGS=-T system/linker/STM32F103C8T6_Linker.ld -g -nostdlib -Wl,-Map=system/build/project.map
set INCLUDES=-I./system/CMSIS -I./bsp/inc -I./main/inc -I./app/inc -I./DSA_utils/inc -I./library/inc

REM ===== Output =====
set BUILD_DIR=system/build
if not exist %BUILD_DIR% mkdir %BUILD_DIR%

REM =========================
REM 1. Compile
REM =========================
set OBJECTS=

for /r %%f in (*.c) do (
    set "SRC_FILE=%%f"

    set "OBJ_FILE=!SRC_FILE:%CD%=!"
    set "OBJ_FILE=!OBJ_FILE:.c=.o!"
    set "OBJ_FILE=%BUILD_DIR%!OBJ_FILE!"
    set "OBJ_FILE=!OBJ_FILE:/=\!"

    for %%d in ("!OBJ_FILE!") do if not exist "%%~dpd" mkdir "%%~dpd"

    echo Compiling: %%f

    %CC% %CFLAGS% %INCLUDES% "%%f" -o "!OBJ_FILE!"

    if !errorlevel! neq 0 (
        echo ---------------------------------------
        echo ERROR in %%f
        pause
        exit /b !errorlevel!
    )

    set OBJECTS=!OBJECTS! "!OBJ_FILE!"
)

REM =========================
REM 2. Link
REM =========================
echo ---------------------------------------
echo Linking...

%CC% %LDFLAGS% !OBJECTS! -o %BUILD_DIR%\project.elf

if %errorlevel% neq 0 (
    echo LINK FAILED
    pause
    exit /b 1
)

%OBJCOPY% -O binary %BUILD_DIR%\project.elf %BUILD_DIR%\project.bin

echo ---------------------------------------
echo BUILD SUCCESSFUL

REM =========================
REM 3. FLASH APP
REM =========================

choice /M "Flash APP to STM32 now"

if errorlevel 2 goto END

echo ---------------------------------------
echo Flashing application...

%OPENOCD% ^
-f interface/stlink.cfg ^
-f target/stm32f1x.cfg ^
-c "program %BUILD_DIR%/project.bin 0x08001000 verify reset exit"

if %errorlevel% neq 0 (
    echo FLASH FAILED
    pause
    exit /b 1
)

echo ---------------------------------------
echo APP FLASH SUCCESSFUL