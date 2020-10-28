#ifndef __DEFINE_H__
#define __DEFINE_H__

#define FW_VERSION "v0.0.1"

// gpios ------------------------------
#define GPIO_BTN1 	34
#define GPIO_BTN2 	35
#define GPIO_SW1 	32
#define GPIO_LED1 	22
#define GPIO_LED2 	23

//typedef enum {false=0, true} bool;
typedef enum {OFF=0, ON} onoff_t;

// tasks and events ------------------------------
typedef enum
{
	TASK_POLL_IO = 0,
	TASK_BUTTON,
	TASK_LED,
	TASK_STATE_MACHINE,
	MAXNO_TASK
} task_id_t;

typedef enum
{
	EVENT_BTN_MAIN_SHORT=0, 	// 0=highest priority, 31=lowest priority
	EVENT_BTN_MAIN_LONG,
	EVENT_BTN_TEST_SHORT,
	EVENT_BTN_TEST_LONG,
	EVENT_SW_DET_ON,
	EVENT_SW_DET_OFF,
	EVENT_STATE_TIMEOUT,
	MAXNUM_EVENTs,
	EVENT_STATE_POLL=100,
} event_t;

typedef enum
{
	STATE_DISCOVER=0,
	STATE_MODE1,
	STATE_MODE2,
	STATE_CLEANING,
	MAXNUM_STATEs,
	NO_STATE_CHANGE
} state_t;

typedef enum {
	STATE_PHASE_ENTER=0,
	STATE_PHASE_ACTION,
	STATE_PHASE_EXIT,
	MAXNUM_STATE_FUNCs
} state_phase_t;

typedef struct
{
	uint8_t type; 			// solid, blink
	uint8_t init_status; 	// on, off
	uint32_t interval_ms; 	// blink half period
	uint32_t active_ms;		// expire after
} led_message_t;

// message type ------------------------------
typedef struct
{
	uint8_t mode_sw;
	uint32_t batt_mv;
} poll_message_t;

typedef struct
{
	uint8_t id;
	uint8_t event;
} button_message_t;

typedef union
{
	led_message_t led;
	poll_message_t poll;
	button_message_t button;
} message_body_t;

typedef struct
{
	task_id_t from;
	event_t event;
	message_body_t sdu;
} message_t;

// buttons and switchs ------------------------------
#define BTN_DEBOUNCE_TIME 	50 // ms
#define BTN_SHORT_PRESS_TIME_FROM 	10 // ms
#define BTN_SHORT_PRESS_TIME_TO 	500 // ms
#define BTN_LONG_PRESS_TIME 	1000 // ms

#define SW_DEBOUNCE_TIME 	50 // ms

typedef enum
{
	BUTTON_ID_MAIN=0,
	BUTTON_ID_TEST,
	MAXNUM_BUTTON_IDs
} button_t;

typedef enum {
	SWTICH_ID_DET=0,
	MAXNUM_SWITCH_IDs
} switch_t;

typedef enum
{
    POLARITY_ACTIVE_LOW=0,
    POLARITY_ACTIVE_HIGH,
    MAXNUM_POLARITYs,
} button_polarity_t;

typedef struct {
    button_t button_id;
    button_polarity_t polarity;
    uint8_t pin;
    bool pressed;
    bool press_trigger_fired;
    int  press_tick_count;
	event_t event_short;
	event_t event_long;
	event_t event_release;
} button_info_t;

typedef struct {
    switch_t switch_id;
    uint8_t pin;
    bool state_on;
    uint16_t tick_count;
	event_t event_on;
	event_t event_off;
} switch_info_t;

// state machine ------------------------------
typedef state_t (*state_entry_func_t)(event_t);
typedef state_t (*state_action_func_t)(event_t);
typedef void (*state_exit_func_t)(event_t);

typedef struct
{
	state_entry_func_t entry_function;
	state_action_func_t action_function;
	state_exit_func_t exit_function;
} state_functions_t;

#endif
