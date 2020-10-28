/* Poll IO Task

   Update IO status by polling

*/
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
#include "driver/gpio.h"

#include "define.h"
#include "task_poll_io.h"


extern QueueHandle_t xQueueSM;

static switch_info_t switchs[MAXNUM_SWITCH_IDs];
static const int SWITCHS_COUNT = sizeof(switchs)/sizeof(switchs[0]);

void init_switchs(void)
{
	switchs[0].switch_id = SWTICH_ID_DET;
	switchs[0].pin = GPIO_SW1;
	switchs[0].event_on = EVENT_SW_DET_ON;
	switchs[0].event_off = EVENT_SW_DET_OFF;
}

void poll_switch(uint32_t interval_ms)
{
	int i;
	bool st;
	message_t message;

	memset(&message, 0x00, sizeof(message_t));
	message.from = TASK_BUTTON;

	for(i=0; i<SWITCHS_COUNT; i++)
	{
		st = (gpio_get_level(switchs[i].pin)==0)? false: true;

		if(switchs[i].state_on != st)
		{
			switchs[i].state_on = st;
			switchs[i].tick_count=0;
		}
		else
		{
			switchs[i].tick_count++;
			if(switchs[i].tick_count == SW_DEBOUNCE_TIME/interval_ms)
			{
				if(switchs[i].state_on)
				{
					message.event = switchs[i].event_on;
					xQueueSend(xQueueSM, (message_t *)&message, (TickType_t)0);
				}
				else
				{
					message.event = switchs[i].event_off;
					xQueueSend(xQueueSM, (message_t *)&message, (TickType_t)0);
				}
			}
			if(switchs[i].tick_count > SW_DEBOUNCE_TIME/interval_ms)
			{
				switchs[i].tick_count = SW_DEBOUNCE_TIME/interval_ms + 1; // to prevent overflow
			}
		}
	}
}

void task_poll_io(void *pvParameters)
{
	init_switchs();

    while (1)
	{
		poll_switch(10);
		vTaskDelay(10/portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}


