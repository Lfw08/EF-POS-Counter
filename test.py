import serial
import time

def main():
    # 打开串口

    

    ser = serial.Serial(
        port='COM1',       # 替换为你的串口名称，例如 'COM1' 或 '/dev/ttyS0'
        baudrate=9600,     # 波特率
        bytesize=serial.EIGHTBITS, # 数据位
        parity=serial.PARITY_NONE, # 校验位
        stopbits=float(serial.STOPBITS_ONE), # 停止位
        timeout=1          # 读超时时间（秒）
    )

    if not ser.is_open:
        print("Error opening serial port")
        return

    print("Serial port opened successfully")

    try:
        while True:
            if ser.in_waiting > 0:
                data = ser.readline().decode('utf-8').strip()
                print(f"Data read from serial port: {data}")
            time.sleep(0.1)  # 等待一段时间再读取
    except KeyboardInterrupt:
        print("Exiting...")
    finally:
        ser.close()
        print("Serial port closed")

if __name__ == "__main__":
    main()
