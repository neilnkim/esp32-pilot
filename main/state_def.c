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

#include "state_def.h"


extern st_status_t g_st;


state_functions_t state_table[MAXNUM_STATEs];

//caution: do NEVER re-assign
void init_state_table(void)
{
	state_table[STATE_DISCOVER].entry_function 	= state_entry_discover;
	state_table[STATE_DISCOVER].action_function = state_action_discover;
	state_table[STATE_DISCOVER].exit_function 	= state_exit_discover;

	state_table[STATE_MODE1].entry_function  = state_entry_mode1;
	state_table[STATE_MODE1].action_function = state_action_mode1;
	state_table[STATE_MODE1].exit_function 	 = state_exit_mode1;

	state_table[STATE_MODE2].entry_function  = state_entry_mode2;
	state_table[STATE_MODE2].action_function = state_action_mode2;
	state_table[STATE_MODE2].exit_function 	 = state_exit_mode2;

	state_table[STATE_CLEANING].entry_function  = state_entry_cleaning;
	state_table[STATE_CLEANING].action_function = state_action_cleaning;
	state_table[STATE_CLEANING].exit_function	= state_exit_cleaning;
}


// -------------------------------------------------------------
void state_init()
{
	init_state_table();
	set_state(STATE_DISCOVER);
}

void states_update(event_t event)
{
	state_t new_state;

	if(state_table[get_state()].action_function == NULL)
	{
		// never come here
		state_init();
		return;
	}

	new_state = state_table[get_state()].action_function(event);

	while(new_state != NO_STATE_CHANGE)
	{
		g_st.state_tstamp = get_systick();
		if(state_table[get_state()].exit_function != NULL)
		{
			state_table[get_state()].exit_function(event);
		}

		set_state(new_state);
		new_state = NO_STATE_CHANGE;
		if(state_table[get_state()].entry_function != NULL)
		{
			new_state = state_table[get_state()].entry_function(event);
		}
	}
}

void task_states(void)
{
	event_t event;
	event = pop_event();

	if(event != EVENT_STATE_POLL)
	{
		set_time_stamp(&g_st.event_tstamp);
		print_event(event);
	}

	states_update(event);
}

void print_event(event_t event)
{
	printf("[EVENT] ");

	switch(event)
	{
		case EVENT_BTN_MAIN_SHORT:
			printf("BTN_MAIN_CLICK");
			break;
		case EVENT_BTN_MAIN_LONG:
			printf("BTN_MAIN_LONG");
			break;
		case EVENT_BTN_TEST_SHORT:
			printf("BTN_TEST_CLICK");
			break;
		case EVENT_BTN_TEST_LONG:
			printf("BTN_TEST_LONG");
			break;
		case EVENT_SW_DET_ON:
			printf("SW_DET_ON");
			break;
		case EVENT_SW_DET_OFF:
			printf("SW_DET_OFF");
			break;
		case EVENT_STATE_TIMEOUT:
			printf("STATE_TIMEOUT");
			break;
		case EVENT_STATE_POLL:
			printf("STATE_POLL");
			break;
		default:
			printf("???");
			break;
	}

	printf("\n");
}

void print_state(state_phase_t st, state_t state)
{
	printf("[STATE] ");
	switch(st)
	{
		case STATE_PHASE_ENTER:
			printf("ENTER ");
			break;
		case STATE_PHASE_ACTION:
			printf("ACTION ");
			break;
		case STATE_PHASE_EXIT:
			printf("EXIT  ");
			break;
		default:
			printf("??? ");
			break;
	}

	switch(state)
	{
		case STATE_DISCOVER:
			printf("DISCOVER");
			break;
		case STATE_MODE1:
			printf("MODE1");
			break;
		case STATE_MODE2:
			printf("MODE2");
			break;
		case STATE_CLEANING:
			printf("CLEANING");
			break;
		default:
			printf("???");
			break;
	}

	printf("\n");
}

