import serial
import struct
import time

PORT = "COM5"
BAUD = 9600

ser = serial.Serial(
    port=PORT,
    baudrate=BAUD,
    timeout=1
)

ser.reset_input_buffer()
ser.reset_output_buffer()

print("Waiting bootloader...")
while True:
    data = ser.read(1)

    if data == b'U':
        print("Bootloader detected")
        break
ser.write(b'Y')

print("Update request sent")
while True:
    data = ser.read(1)

    if data == b'S':
        print("Bootloader ready")
        break
with open("../Firmware_Application/system/build/project.elf", "rb") as f:
    fw = f.read()

fw_size = len(fw)

print(f"Firmware size: {fw_size} bytes")
ser.write(struct.pack("<I", fw_size))
ser.write(fw)
print("Firmware sent")
ser.close()