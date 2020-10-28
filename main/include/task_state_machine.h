#ifndef __TASK_STATE_MACHINE__H__
#define __TASK_STATE_MACHINE__H__

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

void task_state_machine(void *pvParameters);
void init_state_table(void);
void state_init();
void states_update(event_t event);
void print_event(task_id_t from, event_t event);
void print_state(state_phase_t st, state_t state);

#endif
