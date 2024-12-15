#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <pthread.h>

int cnt = -1, charnum;
char newnum[5], read_buf[256];
pthread_t thread[5];

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
    tty.c_cflag &= ~CSTOPB; // 清除停止位
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

void writeSerialPort(int serial_port, const char* data) {
    ssize_t bytes_written = write(serial_port, data, strlen(data));
    if (bytes_written < 0) {
        std::cerr << "Error writing to serial port: " << strerror(errno) << std::endl;
    }
}

void *readSerialPort(void *serial_port) {
    memset(read_buf, 0, sizeof(read_buf));
    ssize_t bytes_read = read(serial_port, &read_buf, sizeof(read_buf));
    if (bytes_read > 0) {
        std::cout << "Read " << bytes_read << " bytes. Received message: " << std::string(read_buf, bytes_read) << std::endl;
    } else if (bytes_read < 0) {
        std::cerr << "Error reading from serial port: " << strerror(errno) << std::endl;
    }
    pthread_exit(NULL);
}

void *input(){
    newnum[++charnum] = getchar();
    pthread_exit(NULL);
}

int main() {
    freeopen("POS-Data.data", "w", stdout);
    const char* serialPortPath = "/dev/ttyUSB0"; // 串口设备路径

    int serial_port = setupSerialPort(serialPortPath);
    if(serial_port < 0) {
        return 1;
    }

    // 发送数据
    const char* data = "Hello, serial port!";
    writeSerialPort(serial_port, data);

    while(true){
        int rc = pthread_create(&thread[0], NULL, readSerialPort, &serial_port);
        if(rc != 0) cerr << "Serial Input Error\n";
        rc = pthread_create(&thread[1], NULL, input);
        if(rc != 0) cerr << "Keyboard Input Error\n";
        if(read_buf == ){
            cnt--;
            cout << cnt << "\n";
        }
        if(newnum[charnum] == '\n'){
            cnt = 0;
            for(int i = charnum - 1; i >= 0; i--){
                cnt += (newnum - '0') * pow(10, charnum - i - 1);
            }
            memset(newnum, 0, sizeof(newnum));
            cout << cnt << "\n";
        }
        if(cnt == 0)    break;
    }

    // 关闭串口
    close(serial_port);

    return 0;
}
