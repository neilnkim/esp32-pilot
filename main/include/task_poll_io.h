#ifndef __TASK_POLL_IO__H__
#define __TASK_POLL_IO__H__

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

#include "define.h"

void task_poll_io(void *pvParameters);

#endif
