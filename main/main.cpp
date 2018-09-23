#include "esp_log.h"
#include "fonts.h"
#include "ssd1306.hpp"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "sdkconfig.h"
using namespace std;



#define VCC_GPIO GPIO_NUM_5
#define GND_GPIO GPIO_NUM_17
#define SCK_GPIO GPIO_NUM_22
#define SDA_GPIO GPIO_NUM_21

OLED oled = OLED(SCK_GPIO, SDA_GPIO, SSD1306_128x64);

void myTask(void *pvParameters) {

	while (1) {

		oled.clear();
		oled.select_font(2);
		oled.draw_string(0, 0, "Test", WHITE, BLACK);

		oled.refresh(true);
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}

}

#ifdef __cplusplus
extern "C" {
#endif
void app_main() {

	/* Configure the IOMUX register for pad BLINK_GPIO (some pads are
	   muxed to GPIO on reset already, but some default to other
	   functions and need to be switched to GPIO. Consult the
	   Technical Reference for a list of pads and their default
	   functions.)
	*/
	gpio_pad_select_gpio(VCC_GPIO);
	gpio_pad_select_gpio(GND_GPIO);
	/* Set the GPIO as a push/pull output */
	gpio_set_direction(VCC_GPIO, GPIO_MODE_OUTPUT);
	gpio_set_direction(GND_GPIO, GPIO_MODE_OUTPUT);
	/* Set the GPIO to VCC / GND */
	gpio_set_level(VCC_GPIO, 1);
	gpio_set_level(GND_GPIO, 0);


	oled = OLED(SCK_GPIO, SDA_GPIO, SSD1306_128x64);
	if (oled.init()) {
		ESP_LOGI("OLED", "oled inited");
//		oled.draw_rectangle(10, 30, 20, 20, WHITE);
//		oled.select_font(0);
//		//deprecated conversion from string constant to 'char*'
//		oled.draw_string(0, 0, "glcd_5x7_font_info", WHITE, BLACK);
//		ESP_LOGI("OLED", "String length:%d",
//				oled.measure_string("glcd_5x7_font_info"));
//		oled.select_font(1);
//		oled.draw_string(0, 18, "tahoma_8pt_font_info", WHITE, BLACK);
//		ESP_LOGI("OLED", "String length:%d",
//				oled.measure_string("tahoma_8pt_font_info"));
//		oled.draw_string(55, 30, "Hello ESP32!", WHITE, BLACK);
//		oled.refresh(true);
//		vTaskDelay(3000 / portTICK_PERIOD_MS);
		xTaskCreatePinnedToCore(&myTask, "adctask", 2048, NULL, 5, NULL, 1);
	} else {
		ESP_LOGE("OLED", "oled init failed");
	}
}
#ifdef __cplusplus
}
#endif
