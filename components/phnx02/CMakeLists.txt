set(FREERTOS_PORT "freertos/port.c"	"freertos/heap_1.c")
set(STARTUP_SRCS "startup/startup.c" "startup/retarget.c")
file(GLOB DRIVER_SRCS "drivers/*.c")

set(COMPONENT_SRCS ${DRIVER_SRCS} ${STARTUP_SRCS})
set(COMPONENT_ADD_INCLUDEDIRS "include drivers")

if(WITH_FREERTOS)
	set(COMPONENT_SRCS ${COMPONENT_SRCS} ${FREERTOS_PORT})
	set(COMPONENT_ADD_INCLUDEDIRS "${COMPONENT_ADD_INCLUDEDIRS} freertos")
	set(COMPONENT_REQUIRES "freertos")
endif()

register_component()

# 链接本库的依赖
target_link_directories(${COMPONENT} PUBLIC "ldscripts")
target_link_libraries(${COMPONENT} "-T MKL25Z4.ld")
