
set(STARTUP_SRCS "RISCV/env_Eclipse/handlers.c" "RISCV/env_Eclipse/entry.S" "RISCV/env_Eclipse/start.S")
file(GLOB_RECURSE DRIVER_SRCS "GD32VF103_standard_peripheral/*.c")
file(GLOB_RECURSE USBFS_DRIVER_SRCS "GD32VF103_usbfs_driver/*.c")
file(GLOB_RECURSE STUBS  "RISCV/stubs/*.c" "RISCV/env_Eclipse/init.c")
set(COMPONENT_SRCS ${DRIVER_SRCS} ${STARTUP_SRCS} "RISCV/drivers/n200_func.c")
set(COMPONENT_EXTRA_SRCS ${STUBS})
set(COMPONENT_ADD_INCLUDEDIRS ". RISCV/drivers GD32VF103_standard_peripheral GD32VF103_standard_peripheral/Include GD32VF103_usbfs_driver/Include")

register_component()

# 链接本库的依赖
target_link_directories(${COMPONENT} PUBLIC "RISCV/env_Eclipse")
target_link_libraries(${COMPONENT} "-T GD32VF103x8.lds")
