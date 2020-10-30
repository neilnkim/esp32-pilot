/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"

#include "define.h"
#include "task_led.h"
#include "task_poll_io.h"
#include "task_button.h"
#include "task_state_machine.h"
#include "timer_func.h"


void init_gpio(void)
{
    gpio_set_direction(GPIO_SW1, GPIO_MODE_INPUT);
    gpio_set_direction(GPIO_BTN1, GPIO_MODE_INPUT);
    gpio_set_direction(GPIO_BTN2, GPIO_MODE_INPUT);

	gpio_pullup_en(GPIO_SW1);
	gpio_pullup_en(GPIO_BTN1);
	gpio_pullup_en(GPIO_BTN2);

	gpio_set_direction(GPIO_LED1, GPIO_MODE_OUTPUT);
	gpio_set_direction(GPIO_LED2, GPIO_MODE_OUTPUT);
}
/*
gpio_get_level(GPIO_SW1);
gpio_get_level(GPIO_BTN1);
gpio_get_level(GPIO_BTN2);

*/

void print_version(void)
{
    printf("Pilot project %s -----\n", FW_VERSION);
}

#ifdef CONFIG_IDF_TARGET_ESP32
#define CHIP_NAME "ESP32"
#endif

#ifdef CONFIG_IDF_TARGET_ESP32S2BETA
#define CHIP_NAME "ESP32-S2 Beta"
#endif
void print_chip_info(void)
{
	esp_chip_info_t chip_info;

	esp_chip_info(&chip_info);

	printf("This is %s chip with %d CPU cores, WiFi%s%s, ",
		CHIP_NAME,
		chip_info.cores,
		(chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
		(chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
	printf("silicon revision %d, ", chip_info.revision);
	printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
		(chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
}


void app_main(void)
{
	init_gpio();
	init_state_timer();

	print_version();
	print_chip_info();

	xTaskCreate(task_poll_io, 	"poll_io", 	2048, NULL, tskIDLE_PRIORITY+1, NULL);
	xTaskCreate(task_led,		"led", 		2048, NULL, tskIDLE_PRIORITY+1, NULL);
	xTaskCreate(task_button, 	"button", 	2048, NULL, tskIDLE_PRIORITY+1, NULL);
	xTaskCreate(task_state_machine, "state_machine", 2048, NULL, tskIDLE_PRIORITY+1, NULL);

#if 0 // uncomment these causes panic, why?
	vTaskStartScheduler();
	for(;;){}
#endif
}

