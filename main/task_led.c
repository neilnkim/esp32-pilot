/* Poll IO Task

   Update IO status by polling

*/

#include <stdio.h>
#include <string.h>
#include <sys/param.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"

#include "define.h"
#include "task_led.h"

QueueHandle_t xQueueLed;

void task_led(void *pvParameters)
{
	led_message_t led_message;

	xQueueLed = xQueueCreate(5, sizeof(led_message_t));

    while (1)
	{
		if(xQueueReceive( xQueueLed, (led_message_t *)&led_message, (TickType_t)100 ) )
		{
	        printf("[task_led] recv message = %d, %d, %d, %d\n",
	        	led_message.type, led_message.init_status, led_message.interval_ms, led_message.active_ms );

			//todo: init led process
		}

		//todo: led process

//		gpio_set_pin(GPIO_LED1);
//		gpio_reset_pin(GPIO_LED2);

		vTaskDelay( 10/portTICK_PERIOD_MS );
	}

    vTaskDelete(NULL);
}


