idf_component_register(
	SRCS 			"pilot_main.c"
					"task_poll_io.c"
					"task_led.c"
					"task_button.c"
					"task_state_machine.c"
					"state_func.c"
    INCLUDE_DIRS 	"."
					"include"
	)
