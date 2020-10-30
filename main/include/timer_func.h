#ifndef __TIMER_FUNC__H__
#define __TIMER_FUNC__H__

#include "define.h"
#include "esp_timer.h"


void init_state_timer(void);
void set_state_timeout(uint32_t timeout_ms);
void clear_state_timeout(void);
void oneshot_timer_callback(void* arg);

#endif

