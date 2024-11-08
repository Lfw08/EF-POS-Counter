#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
using namespace std;
int OpenSerial(string tty){
    int serialport = open(tty, O_RDWR | O_NOCTTY | O_NDELAY);
    if (serialport == -1) {
        std::cerr << "Error opening serial port" << std::endl;
    }
    return serialport;
}
void SetSerial(int serialport){
    struct termios options;
    tcgetattr(serialport, &options);
    cfsetispeed(&options, B9600); // 输入波特率
    cfsetospeed(&options, B9600); // 输出波特率
    options.c_cflag |= (CLOCAL | CREAD); // 启用接收器和本地模式
    options.c_cflag &= ~CSIZE; // 清除当前字符大小位
    options.c_cflag |= CS8; // 8位数据位
    options.c_cflag &= ~PARENB; // 无校验位
    options.c_cflag &= ~CSTOPB; // 1个停止位
    options.c_cc[VMIN] = 1; // 最小读取字符数
    options.c_cc[VTIME] = 0; // 读取超时（以十分之一秒为单位）
    tcsetattr(serialport, TCSANOW, &options);
}
int main() {
    int Input_serialPort = OpenSerial("/dev/ttyS0");
    int Output_serialPort = OpenSerial("/dev/ttyS1")
    SetSerial(Input_serialPort);
    SetSerial(Output_serialPort);

    // 读取数据
    char buffer[128];
    memset(buffer,0,sizeof(buffer));
    int bytesRead = read(Input_serialPort, buffer, sizeof(buffer));
    if (bytesRead > 0) {
        cout << "Data read from serial port: " << string(buffer, bytesRead) << "\n";
    } else {
        cerr << "Error reading from serial port\n";
        system("pause");
    }
    int bytesWrite = write(Output_serialPort, buffer, sizeof(buffer));


    // 关闭串口
    close(Input_serialPort);
    close(Output_serialPort);
    //test redirect
    return 0;
}
