set(FREERTOS_PORT "freertos/port.c"	"freertos/portASM_i.S" "freertos/heap_3.c")
set(STARTUP_SRCS 
	"startup/entry.S"
	"startup/init.c"
	"startup/start.S"
	"startup/handlers.c"
	"startup/retarget.c"
	"startup/system_gd32vf103.c"
)

file(GLOB DRIVER_SRCS "drivers/*.c" "peripheral/*.c")

set(COMPONENT_SRCS ${DRIVER_SRCS} ${STARTUP_SRCS})
set(COMPONENT_ADD_INCLUDEDIRS "include" "drivers" "peripheral")

if(WITH_FREERTOS)
	set(COMPONENT_SRCS ${COMPONENT_SRCS} ${FREERTOS_PORT})
	set(COMPONENT_ADD_INCLUDEDIRS "${COMPONENT_ADD_INCLUDEDIRS} freertos")
	set(COMPONENT_REQUIRES "freertos")
endif()

register_component()

# 链接本库的依赖
target_link_directories(${COMPONENT} PUBLIC "ldscripts")
target_link_libraries(${COMPONENT} "-T GD32VF103x8.lds")

