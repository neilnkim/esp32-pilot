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
#include "state_func.h"


// -------------------------------------------------------------
QueueHandle_t xQueueSM;

void task_state_machine(void *pvParameters)
{
	message_t sm_message;

	xQueueSM = xQueueCreate(10, sizeof(message_t));
	state_init();

	while(1)
	{
		sm_message.event = EVENT_STATE_POLL;

		if(xQueueReceive( xQueueSM, (message_t *)&sm_message, (TickType_t)5 ) )
		{
			printf("[task_sm] recv message = %d, %d\n", sm_message.from, sm_message.event);
		}

		if(sm_message.event != EVENT_STATE_POLL)
		{
//			set_time_stamp(&g_st.event_tstamp);
			print_event(sm_message.from, sm_message.event);
		}

		states_update(sm_message.event);
	}

	vTaskDelete(NULL);
}


// -------------------------------------------------------------
state_functions_t state_table[MAXNUM_STATEs];
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
//		g_st.state_tstamp = get_systick();
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


// -------------------------------------------------------------
void print_event(task_id_t from, event_t event)
{
	printf("[EVENT] from task: ");

	switch(from)
	{
		case TASK_POLL_IO:
			printf("POLL_IO");
			break;
		case TASK_BUTTON:
			printf("BUTTON");
			break;
		case TASK_LED:
			printf("LED");
			break;
		case TASK_STATE_MACHINE:
			printf("STATE_MACHINE");
			break;
		default:
			printf("unknown");
			break;
	}

	printf(", event: ");

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
			printf("unknown");
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
			printf("unknown ");
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
			printf("unknown");
			break;
	}

	printf("\n");
}


