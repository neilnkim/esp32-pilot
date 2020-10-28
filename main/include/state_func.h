#ifndef __STATE_FUNC__H__
#define __STATE_FUNC__H__

#include "define.h"
//#include "state_def.h"

state_t state_entry_discover(event_t event);
state_t state_action_discover(event_t event);
void state_exit_discover(event_t event);
state_t state_entry_mode1(event_t event);
state_t state_action_mode1(event_t event);
void state_exit_mode1(event_t event);
state_t state_entry_mode2(event_t event);
state_t state_action_mode2(event_t event);
void state_exit_mode2(event_t event);
state_t state_entry_cleaning(event_t event);
state_t state_action_cleaning(event_t event);
void state_exit_cleaning(event_t event);

state_t get_state(void);
void set_state(state_t state);

#endif

