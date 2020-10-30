/*

*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#include "define.h"
#include "task_led.h"
#include "task_poll_io.h"
#include "task_button.h"
#include "task_state_machine.h"
#include "timer_func.h"

extern QueueHandle_t xQueueSM;

static esp_timer_create_args_t oneshot_timer_args;
static esp_timer_handle_t oneshot_timer;

static const char* TAG = "timer";

void init_state_timer(void)
{
	event_t timeout_event = EVENT_STATE_TIMEOUT;

	oneshot_timer_args.callback = &oneshot_timer_callback;
	oneshot_timer_args.arg = (void*) &timeout_event;
	oneshot_timer_args.name = "one-shot";

	esp_timer_create(&oneshot_timer_args, &oneshot_timer);

//    ESP_ERROR_CHECK(esp_timer_delete(oneshot_timer));

	ESP_LOGI(TAG, "one-shot timer initialized: %lld us", esp_timer_get_time());
}

void set_state_timeout(uint32_t timeout_ms)
{
	uint64_t timeout_us = (uint64_t)timeout_ms*1000;

	esp_timer_start_once(oneshot_timer, timeout_us);

	ESP_LOGI(TAG, "one-shot timer started: %lld us", esp_timer_get_time());
}

void clear_state_timeout(void)
{
	esp_timer_stop(oneshot_timer);

	ESP_LOGI(TAG, "one-shot timer stopped: %lld us", esp_timer_get_time());
}

void oneshot_timer_callback(void* arg)
{
	message_t message;

	memset(&message, 0x00, sizeof(message_t));
	message.from = TASK_STATE_MACHINE;
	message.event = EVENT_STATE_TIMEOUT;

	xQueueSend(xQueueSM, (message_t *)&message, (TickType_t)0);
	ESP_LOGI(TAG, "one-shot timer expired: %lld us", esp_timer_get_time());
	printf("\n");
//	set_state_timeout(15000);
}


