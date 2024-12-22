import time
from rgbmatrix import RGBMatrix, RGBMatrixOptions

# 配置LED屏幕参数
options = RGBMatrixOptions()
options.rows = 32  # 根据你的屏幕实际行数修改
options.cols = 64  # 根据你的屏幕实际列数修改
options.chain_length = 1
options.parallel = 1
options.hardware_mapping = 'adafruit-hat'  # 根据你的硬件连接情况修改
options.scan_mode = 20  # 设置为二十分之一扫描

# 创建RGBMatrix对象
matrix = RGBMatrix(options=options)

# 创建一个画布
canvas = matrix.CreateFrameCanvas()

try:
    while True:
        # 在画布上绘制图形或文字
        canvas.Fill(0, 0, 0)  # 先清空画布
        canvas.SetPixel(10, 10, 255, 0, 0)  # 在坐标(10, 10)处绘制一个红色像素点
        canvas.SetPixel(20, 20, 0, 255, 0)  # 在坐标(20, 20)处绘制一个绿色像素点
        canvas.SetPixel(30, 30, 0, 0, 255)  # 在坐标(30, 30)处绘制一个蓝色像素点

        # 将画布内容显示在LED屏幕上
        matrix.SwapOnVSync(canvas)

        time.sleep(0.1)
except KeyboardInterrupt:
    pass
finally:
    # 清理资源
    matrix.Clear()
    del matrix