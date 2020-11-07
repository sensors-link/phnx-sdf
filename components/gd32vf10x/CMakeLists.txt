
set(STARTUP_SRCS "RISCV/env_Eclipse/handlers.c" "RISCV/env_Eclipse/entry.S" "RISCV/env_Eclipse/start.S")
file(GLOB_RECURSE DRIVER_SRCS "GD32VF103_standard_peripheral/*.c")
file(GLOB_RECURSE USBFS_DRIVER_SRCS "GD32VF103_usbfs_driver/*.c")
file(GLOB_RECURSE STUBS  "RISCV/stubs/*.c" "RISCV/env_Eclipse/init.c")

# COMPONENT CMakeList.txt说明

# COMPONENT_SRCS: 本组件的源文件列表, 以库的形式链接
set(COMPONENT_SRCS ${DRIVER_SRCS})

# COMPONENT_EXTRA_SRCS: 对于TARGET平台组件, stubs和启动代码应直接链接到主目标文件
set(COMPONENT_EXTRA_SRCS ${STUBS} ${STARTUP_SRCS} "RISCV/drivers/n200_func.c")

# COMPONENT_ADD_INCLUDEDIRS: 引出到应用工程的include目录
set(COMPONENT_ADD_INCLUDEDIRS ". RISCV/drivers GD32VF103_standard_peripheral GD32VF103_standard_peripheral/Include GD32VF103_usbfs_driver/Include")

# COMPONENT_PRIV_INCLUDEDIRS: 编译本组件需要的内部头文件目录
set(COMPONENT_PRIV_INCLUDEDIRS "")

# COMPONENT_REQUIRES: 本组件依赖的其他组件
set(COMPONENT_REQUIRES "")

register_component()

# 对于TARGET平台组件，如果工程没有指定LDSCRIPT，在此指定
if (NOT PROJECT_LDSCRIPT)
    target_link_directories(${COMPONENT} PUBLIC "RISCV/env_Eclipse")
    target_link_libraries(${COMPONENT} "-T GD32VF103x8.lds")
endif()
