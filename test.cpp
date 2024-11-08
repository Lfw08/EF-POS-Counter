//fuck ccf
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
using namespace std;
int main() {
    // 打开串口:ttyS0
    int serialPort = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (serialPort == -1) {
        std::cerr << "Error opening serial port" << std::endl;
        return 1;
    }

    // 配置串口参数
    struct termios options;
    tcgetattr(serialPort, &options);
    cfsetispeed(&options, B9600); // 输入波特率
    cfsetospeed(&options, B9600); // 输出波特率
    options.c_cflag |= (CLOCAL | CREAD); // 启用接收器和本地模式
    options.c_cflag &= ~CSIZE; // 清除当前字符大小位
    options.c_cflag |= CS8; // 8位数据位
    options.c_cflag &= ~PARENB; // 无校验位
    options.c_cflag &= ~CSTOPB; // 1个停止位
    options.c_cc[VMIN] = 1; // 最小读取字符数
    options.c_cc[VTIME] = 0; // 读取超时（以十分之一秒为单位）
    tcsetattr(serialPort, TCSANOW, &options);

    // 读取数据
    char buffer[128];
    memset(buffer,0,sizeof(buffer));
    int bytesRead = read(serialPort, buffer, sizeof(buffer));
    if (bytesRead > 0) {
        cout << "Data read from serial port: " << string(buffer, bytesRead) << "\n";
    } else {
        cerr << "Error reading from serial port\n";
        system("pause");
    }
    int bytesWrite = write(serialPort, buffer, sizeof(buffer));


    // 关闭串口
    close(serialPort);
    //test redirect
    return 0;
}
