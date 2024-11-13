#include <fstream>
#include <string>

void exportGPIO(const std::string& gpio) {
    std::ofstream exportFile("/sys/class/gpio/export");
    if (exportFile.is_open()) {
        exportFile << gpio;
        exportFile.close();
    } else {
        // 文件打开失败，可能是权限问题
    }
}

void unexportGPIO(const std::string& gpio) {
    std::ofstream unexportFile("/sys/class/gpio/unexport");
    if (unexportFile.is_open()) {
        unexportFile << gpio;
        unexportFile.close();
    } else {
        // 文件打开失败，可能是权限问题
    }
}

void setGPIODirection(const std::string& gpio, const std::string& direction) {
    std::ofstream directionFile("/sys/class/gpio/gpio" + gpio + "/direction");
    if (directionFile.is_open()) {
        directionFile << direction;
        directionFile.close();
    } else {
        // 文件打开失败，可能是权限问题或者GPIO编号错误
    }
}

void setGPIOValue(const std::string& gpio, const std::string& value) {
    std::ofstream valueFile("/sys/class/gpio/gpio" + gpio + "/value");
    if (valueFile.is_open()) {
        valueFile << value;
        valueFile.close();
    } else {
        // 文件打开失败，可能是权限问题或者GPIO编号错误
    }
}

std::string getGPIODirection(const std::string& gpio) {
    std::string direction;
    std::ifstream directionFile("/sys/class/gpio/gpio" + gpio + "/direction");
    if (directionFile.is_open()) {
        directionFile >> direction;
        directionFile.close();
    } else {
        // 文件打开失败，可能是权限问题或者GPIO编号错误
    }
    return direction;
}

std::string getGPIOValue(const std::string& gpio) {
    std::string value;
    std::ifstream valueFile("/sys/class/gpio/gpio" + gpio + "/value");
    if (valueFile.is_open()) {
        valueFile >> value;
        valueFile.close();
    } else {
        // 文件打开失败，可能是权限问题或者GPIO编号错误
    }
    return value;
}

int main() {
    std::string gpio = "8"; // 要操作的GPIO编号
    exportGPIO(gpio); // 导出GPIO到用户空间
    setGPIODirection(gpio, "out"); // 设置为输出
    setGPIOValue(gpio, "1"); // 输出高电平
    unexportGPIO(gpio);// 取消导出

    std::string gpio = "9"; // 要操作的GPIO编号
    exportGPIO(gpio);   // 导出GPIO到用户空间
    setGPIODirection(gpio, "in"); // 设置为输入
    std::string direction = getGPIODirection(gpio); // 获取方向
    std::string value = getGPIOValue(gpio); // 获取值
    std::cout << "GPIO" << gpio << " direction: " << direction << ", value: " << value << std::endl;
    unexportGPIO(gpio);// 取消导出
    return 0;   // 程序正常退出
}