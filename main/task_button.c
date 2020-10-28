/* Button Task

   Update Button Events

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
#include "driver/gpio.h"

#include "define.h"
#include "task_button.h"


extern QueueHandle_t xQueueSM;

static button_info_t buttons[MAXNUM_BUTTON_IDs];
static const int BUTTON_COUNT = sizeof(buttons)/sizeof(buttons[0]);

void init_buttons(void)
{
	buttons[0].button_id = BUTTON_ID_MAIN;
	buttons[0].polarity = POLARITY_ACTIVE_LOW;
	buttons[0].pin = GPIO_BTN1;
	buttons[0].event_short = EVENT_BTN_MAIN_SHORT;
	buttons[0].event_long = EVENT_BTN_MAIN_LONG;

	buttons[1].button_id = BUTTON_ID_TEST;
	buttons[1].polarity = POLARITY_ACTIVE_LOW;
	buttons[1].pin = GPIO_BTN2;
	buttons[1].event_short = EVENT_BTN_TEST_SHORT;
	buttons[1].event_long = EVENT_BTN_TEST_LONG;
}

void update_button_status(uint32_t interval_ms)
{
	int i;
	message_t message;

	memset(&message, 0x00, sizeof(message_t));
	message.from = TASK_POLL_IO;

	for(i=0; i<BUTTON_COUNT; i++)
	{
		bool high = (gpio_get_level(buttons[i].pin)==0)? false: true;
		if (( high && buttons[i].polarity == POLARITY_ACTIVE_HIGH) ||
			(!high && buttons[i].polarity == POLARITY_ACTIVE_LOW)		)
		{
			if(buttons[i].pressed)
				continue;

			buttons[i].press_tick_count = 0;
			buttons[i].pressed = true;
		}
		else
		{
			if(!buttons[i].pressed)
				continue;

			if(buttons[i].press_trigger_fired)
			{
				buttons[i].press_trigger_fired = false;
				if(	BTN_SHORT_PRESS_TIME_FROM/interval_ms <= buttons[i].press_tick_count &&
					buttons[i].press_tick_count <= BTN_SHORT_PRESS_TIME_TO/interval_ms)
				{
					message.event = buttons[i].event_short;
					xQueueSend(xQueueSM, (message_t *)&message, (TickType_t)0);
				}
			}
			buttons[i].pressed = false;
		}
	}
}

void poll_button(uint32_t interval_ms)
{
	int i;
	message_t message;

	memset(&message, 0x00, sizeof(message_t));
	message.from = TASK_POLL_IO;

	update_button_status(interval_ms);

    for(i=0; i<BUTTON_COUNT; i++)
    {
        if((buttons[i].pressed) && (buttons[i].press_tick_count < BTN_LONG_PRESS_TIME/interval_ms))
        {
            buttons[i].press_tick_count++;
            if(buttons[i].press_tick_count == BTN_DEBOUNCE_TIME/interval_ms)
            {
                buttons[i].press_trigger_fired = true;
				message.event = buttons[i].event_short;
				xQueueSend(xQueueSM, (message_t *)&message, (TickType_t)0);
            }

            if(buttons[i].press_tick_count == BTN_LONG_PRESS_TIME/interval_ms)
            {
				message.event = buttons[i].event_long;
				xQueueSend(xQueueSM, (message_t *)&message, (TickType_t)0);
            }
        }
    }
}


#define BUTTON_POLL_PERIOD_MS 10
void task_button(void *pvParameters)
{
	init_buttons();

    while (1)
	{
		poll_button(BUTTON_POLL_PERIOD_MS);
		vTaskDelay(BUTTON_POLL_PERIOD_MS/portTICK_PERIOD_MS);
	}

    vTaskDelete(NULL);
}


