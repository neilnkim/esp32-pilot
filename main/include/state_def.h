#ifndef __STATE_DEF_H__
#define __STATE_DEF_H__

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

void init_state_table(void);
void state_init(void);
void states_update(event_t event);
void task_states(void);
void print_event(event_t event);
void print_state(state_phase_t st, state_t state);

#endif

