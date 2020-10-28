
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
#include "state_func.h"
#include "task_state_machine.h"


static state_t current_state;

// -------------------------------------------------------------
state_t state_entry_discover(event_t event)
{
	printf("version = (%s)\n", FW_VERSION);
	print_state(STATE_PHASE_ENTER, STATE_DISCOVER);
/*
	reset_poll();
	set_heater_en(OFF);
	set_led_action_solid(LED_ID1_DEEP, LED_COLOR_OFF, 0);
	set_led_action_solid(LED_ID2_MAKEUP, LED_COLOR_OFF, 0);
	reset_all_timer();
*/
	return NO_STATE_CHANGE;
}

state_t state_action_discover(event_t event)
{
	state_t ret;
	ret = NO_STATE_CHANGE;

	switch(event)
	{
		case EVENT_STATE_POLL:
			break;
		case EVENT_BTN_MAIN_SHORT:
			ret = STATE_MODE1;
			break;
		case EVENT_BTN_MAIN_LONG:
			break;
		case EVENT_BTN_TEST_SHORT:
			break;
		case EVENT_BTN_TEST_LONG:
			break;
		case EVENT_SW_DET_ON:
			break;
		case EVENT_SW_DET_OFF:
			break;
		case EVENT_STATE_TIMEOUT:
			break;
		default:
			break;
	}

#if 0 //#ifdef ENABLE_POWER_DOWN
	if(ret == NO_STATE_CHANGE && !g_st.port_input.plug_usb)
	{
		if(get_time_interval(g_st.event_tstamp) >= 10000)
		{
			g_st.event_tstamp = get_systick();

			led_unit_all_onoff(eOFF);
			PowerDownFunction();
		}
	}
#endif

	return ret;
}

void state_exit_discover(event_t event)
{
	print_state(STATE_PHASE_EXIT, STATE_DISCOVER);
}

// -------------------------------------------------------------
state_t state_entry_mode1(event_t event)
{
	print_state(STATE_PHASE_ENTER, STATE_MODE1);
/*
	set_heater_en(ON);
	set_led_action_blink(LED_ID1_DEEP,   LED_COLOR_DEFAULT, LED_COLOR_OFF, 500, 0);
	set_led_action_solid(LED_ID2_MAKEUP, LED_COLOR_OFF, 0);
	reset_all_timer();
	set_timer(TIMER_STEAM_OFF, 60000, EVENT_STATE_TIMEOUT);
*/
	return NO_STATE_CHANGE;
}

state_t state_action_mode1(event_t event)
{
	state_t ret;
	ret = NO_STATE_CHANGE;

	switch(event)
	{
		case EVENT_STATE_POLL:
			break;
		case EVENT_BTN_MAIN_SHORT:
			ret = STATE_MODE2;
			break;
		case EVENT_BTN_MAIN_LONG:
			ret = STATE_CLEANING;
			break;
		case EVENT_BTN_TEST_SHORT:
			break;
		case EVENT_BTN_TEST_LONG:
			break;
		case EVENT_SW_DET_ON:
			break;
		case EVENT_SW_DET_OFF:
			break;
		case EVENT_STATE_TIMEOUT:
			ret = STATE_DISCOVER;
			break;
		default:
			break;
	}

	return ret;
}

void state_exit_mode1(event_t event)
{
	print_state(STATE_PHASE_EXIT, STATE_MODE1);
/*
	set_heater_en(OFF);
	set_led_action_solid(LED_ID1_DEEP, LED_COLOR_OFF, 0);
	set_led_action_solid(LED_ID2_MAKEUP, LED_COLOR_OFF, 0);
	reset_all_timer();
*/
}


// -------------------------------------------------------------
state_t state_entry_mode2(event_t event)
{
	print_state(STATE_PHASE_ENTER, STATE_MODE2);
/*
	set_heater_en(ON);
	set_led_action_solid(LED_ID1_DEEP, LED_COLOR_OFF, 0);
	set_led_action_blink(LED_ID2_MAKEUP,   LED_COLOR_DEFAULT, LED_COLOR_OFF, 500, 0);
	reset_all_timer();
	set_timer(TIMER_STEAM_OFF, 60000, EVENT_STATE_TIMEOUT);
*/
	return NO_STATE_CHANGE;
}

state_t state_action_mode2(event_t event)
{
	state_t ret;
	ret = NO_STATE_CHANGE;

	switch(event)
	{
		case EVENT_STATE_POLL:
			break;
		case EVENT_BTN_MAIN_SHORT:
			ret = STATE_DISCOVER;
			break;
		case EVENT_BTN_MAIN_LONG:
			ret = STATE_CLEANING;
			break;
		case EVENT_BTN_TEST_SHORT:
			break;
		case EVENT_BTN_TEST_LONG:
			break;
		case EVENT_SW_DET_ON:
			break;
		case EVENT_SW_DET_OFF:
			break;
		case EVENT_STATE_TIMEOUT:
			ret = STATE_DISCOVER;
			break;
		default:
			break;
	}

	return ret;
}

void state_exit_mode2(event_t event)
{
	print_state(STATE_PHASE_EXIT, STATE_MODE2);
/*
	set_heater_en(OFF);
	set_led_action_solid(LED_ID1_DEEP, LED_COLOR_OFF, 0);
	set_led_action_solid(LED_ID2_MAKEUP, LED_COLOR_OFF, 0);
	reset_all_timer();
*/
}

// -------------------------------------------------------------
state_t state_entry_cleaning(event_t event)
{
	print_state(STATE_PHASE_ENTER, STATE_CLEANING);
/*
	set_heater_en(ON);
	set_led_action_blink(LED_ID1_DEEP,   LED_COLOR_OFF,     LED_COLOR_DEFAULT, 100, 0);
	set_led_action_blink(LED_ID2_MAKEUP, LED_COLOR_DEFAULT, LED_COLOR_OFF,     100, 0);
	reset_all_timer();
	set_timer(TIMER_STEAM_OFF, 60000, EVENT_STATE_TIMEOUT);
*/
	return NO_STATE_CHANGE;
}

state_t state_action_cleaning(event_t event)
{
	state_t ret;
	ret = NO_STATE_CHANGE;

	switch(event)
	{
		case EVENT_STATE_POLL:
			break;
		case EVENT_BTN_MAIN_SHORT:
			ret = STATE_DISCOVER;
			break;
		case EVENT_BTN_MAIN_LONG:
			break;
		case EVENT_BTN_TEST_SHORT:
			break;
		case EVENT_BTN_TEST_LONG:
			break;
		case EVENT_SW_DET_ON:
			break;
		case EVENT_SW_DET_OFF:
			break;
		case EVENT_STATE_TIMEOUT:
			ret = STATE_DISCOVER;
			break;
		default:
			break;
	}

	return ret;
}

void state_exit_cleaning(event_t event)
{
	print_state(STATE_PHASE_EXIT, STATE_CLEANING);
/*
	set_heater_en(OFF);
	set_led_action_solid(LED_ID1_DEEP, LED_COLOR_OFF, 0);
	set_led_action_solid(LED_ID2_MAKEUP, LED_COLOR_OFF, 0);
	reset_all_timer();
*/
}


// -------------------------------------------------------------
state_t get_state(void)
{
	return current_state;
}
void set_state(state_t state)
{
	current_state = state;
}



