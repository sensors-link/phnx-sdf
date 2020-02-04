set(FREERTOS_PORT
	"freertos/port.c"
	"freertos/heap_3.c"
)
set(STARTUP_SRCS "startup/startup.c" "startup/delay.c" "startup/retarget.c")
file(GLOB DRIVER_SRCS "drvlib/*.c")

set(COMPONENT_SRCS ${DRIVER_SRCS} ${STARTUP_SRCS})
set(COMPONENT_ADD_INCLUDEDIRS "include include/cmsis drvlib")

if(WITH_FREERTOS)
	set(COMPONENT_SRCS ${COMPONENT_SRCS} ${FREERTOS_PORT})
	set(COMPONENT_ADD_INCLUDEDIRS "${COMPONENT_ADD_INCLUDEDIRS} freertos")
	set(COMPONENT_REQUIRES "freertos")
endif()

register_component()

# 链接本库的依赖
target_link_directories(${COMPONENT} PUBLIC "ldscripts")
target_link_libraries(${COMPONENT} "-T phoenix.ld")
