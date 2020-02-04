# FreeRTOS sources
set(COMPONENT_SRCS
	"list.c" "queue.c" "tasks.c"
	"timers.c"
	"event_groups.c"
	"croutine.c"
	"stream_buffer.c"
)
set(COMPONENT_ADD_INCLUDEDIRS "include include/freertos")
set(COMPONENT_REQUIRES "${TARGET}")

register_component()
