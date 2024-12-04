#include <wiringPi.h>
#include <iostream>
#include <vector>
#include "ws2811.h"

#define LED_COUNT 8  // LED灯珠数量
#define GPIO_PIN 18  // 连接LED的数据引脚

ws2811_led_t led_color;

struct ws2811_leds
{
    ws2811_led_t colors[LED_COUNT];
};

void setLEDColor(ws2811_led_t *leds, int index, uint8_t r, uint8_t g, uint8_t b)
{
    leds[index] = ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

int main()
{
    ws2811_t ledstring =
            {
                    .freq = WS2811_TARGET_FREQ,
                    .dmanum = 10,
                    .channel =
                            {
                                    [0] =
                                            {
                                                    .gpionum = GPIO_PIN,
                                                    .count = LED_COUNT,
                                                    .invert = 0,
                                                    .brightness = 255,
                                                    .strip_type = WS2811_STRIP_GRB
                                            },
                                    [1] =
                                            {
                                                    .gpionum = 0,
                                                    .count = 0,
                                                    .invert = 0,
                                                    .brightness = 0
                                            }
                            }
            };

    ws2811_leds leds;

    if (wiringPiSetup() == -1)
    {
        std::cerr << "wiringPi setup failed" << std::endl;
        return -1;
    }

    if (ws2811_init(&ledstring) != WS2811_SUCCESS)
    {
        std::cerr << "ws2811 init failed" << std::endl;
        return -1;
    }

    // 设置LED颜色
    for (int i = 0; i < LED_COUNT; ++i)
    {
        setLEDColor(leds.colors, i, 255, 0, 0);  // 红色
    }

    // 更新LED状态
    ledstring.channel[0].leds = leds.colors;
    ws2811_render(&ledstring);

    // 延时一段时间
    delay(1000);

    // 关闭LED
    for (int i = 0; i < LED_COUNT; ++i)
    {
        setLEDColor(leds.colors, i, 0, 0, 0);  // 关闭
    }

    ledstring.channel[0].leds = leds.colors;
    ws2811_render(&ledstring);

    ws2811_fini(&ledstring);

    return 0;
}