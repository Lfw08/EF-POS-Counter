//
// Created by qianchengyi on 2025/1/4.
//
#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <cstring>
#include <cmath>
#include <pthread.h>

using namespace std;

int serial_port;

int setupSerialPort(const char* serialPortPath) {
    // 打开串口
    int serial_port = open(serialPortPath, O_RDWR);

    // 检查串口是否打开成功
    if (serial_port < 0) {
        std::cerr << "Error " << errno << " from open: " << strerror(errno) << std::endl;
        return -1;
    }

    // 配置串口参数
    struct termios tty;
    if(tcgetattr(serial_port, &tty) != 0) {
        std::cerr << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
        return -1;
    }

    // 设置波特率为9600 bps
    cfsetospeed(&tty, B9600);
    cfsetispeed(&tty, B9600);

    // 无校验位
    tty.c_cflag &= ~PARENB; // 清除奇偶校验位

    // 设置其他串口参数
    tty.c_cflag &= -CSTOPB; // 1位停止位
    tty.c_cflag &= ~CSIZE; // 清除所有位大小掩码
    tty.c_cflag |= CS8; // 设置8个数据位
    tty.c_cflag &= ~CRTSCTS; // 关闭RTS/CTS流控制
    tty.c_cflag |= CREAD | CLOCAL; // 打开接收器，忽略调制解调器控制线

    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHONL | ISIG); // 关闭规范模式、回显等
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // 关闭软件流控制
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // 关闭特殊处理
    tty.c_oflag &= ~OPOST; // 关闭输出处理
    tty.c_oflag &= ~ONLCR; // 关闭换行转回车换行

    // 保存tty设置，将改变应用于串口
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        std::cerr << "Error " << errno << " from tcsetattr: " << strerror(errno) << std::endl;
        return -1;
    }

    return serial_port;
}

void writeSerialPort(const void *w_data) {
    ssize_t bytes_written = write(serial_port, w_data, 1);
    if (bytes_written < 0) {
        std::cerr << "Error writing to serial port: " << strerror(errno) << std::endl;
    }
    else    cout << "Message sent.\n";
}

int main() {
    const char* serialPortPath = "/dev/ttyAMA0"; // 串口设备路径

    serial_port = setupSerialPort(serialPortPath);
    if(serial_port < 0) {
        return 1;
    }

    // 发送数据
    int Data = 1;
    for(int i = 0; i < 4; i++)    writeSerialPort(&Data);
    return 0;
}
