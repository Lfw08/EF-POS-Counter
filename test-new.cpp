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

int cnt = -1, tmp, charnum, dt[4], serial_port;
char newnum[5], read_buf[256];
pthread_t thread[5];
int _Index[3];

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

void writeSerialPort(const char* w_data) {
    ssize_t bytes_written = write(serial_port, w_data, strlen(w_data));
    if (bytes_written < 0) {
        std::cerr << "Error writing to serial port: " << strerror(errno) << std::endl;
    }
}

void *readSerialPort(void *ccf) {
    while(true){
        memset(dt, 0, sizeof(dt));
        for(int i = 0; i < 4; i++) {
            ssize_t bytes_read = read(serial_port, &dt[i], 1);
            if (bytes_read > 0) {
                std::cout << "Read " << bytes_read << " bytes.\n";
            } else if (bytes_read < 0) {
                std::cerr << "Error reading from serial port: " << strerror(errno) << "\n";
                break;
            }
        }
        int num = dt[1] * 16 + dt[2];
        if(dt[0] == 0x5A && num > tmp && dt[0] || dt[1] || dt[2] == dt[3]){
            tmp = num;
            cnt--;
            cout << cnt << "\n";
        }
    }
    pthread_exit(NULL);
}

void *input(void *ccf){
    while(true) cin >> cnt;
    pthread_exit(NULL);
}

int main() {
    const char* serialPortPath = "/dev/ttyAMA0"; // 串口设备路径

    serial_port = setupSerialPort(serialPortPath);
    if(serial_port < 0) {
        return 1;
    }

    // 发送数据
    const char* w_data = "2";
    writeSerialPort(w_data);
    
    freopen("POS-Data.txt", "w", stdout);
    int rc = pthread_create(&thread[0], NULL, readSerialPort, &_Index[0]);
    if(rc != 0) cerr << "Serial Input Error\n";
    rc = pthread_create(&thread[1], NULL, input, &_Index[1]);
    if(rc != 0) cerr << "Keyboard Input Error\n";

    // 关闭串口
    close(serial_port);

    return 0;
}
