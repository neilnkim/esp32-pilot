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
#include "task_state_machine.h"


QueueHandle_t xQueueSM;

void task_state_machine(void *pvParameters)
{
	message_t sm_message;
	xQueueSM = xQueueCreate(5, sizeof(message_t));

	while (1)
	{
		if( xQueueReceive( xQueueSM, (message_t *)&sm_message, (TickType_t)100 ) )
		{
	        printf("[task_state_machine] recv message = %d, %d\n", sm_message.from, sm_message.event );
		}

		// todo: state machine event handler
//		vTaskDelay( 10 / portTICK_PERIOD_MS);
	}

	vTaskDelete(NULL);
}


