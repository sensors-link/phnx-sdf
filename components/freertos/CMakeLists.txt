# COMPONENT CMakeList.txt说明

# COMPONENT_SRCS: 本组件的源文件列表, 以库的形式链接
file(GLOB COMPONENT_SRCS  "*.c")

# COMPONENT_EXTRA_SRCS: 对于TARGET平台组件, stubs和启动代码应直接链接到主目标文件
# set(COMPONENT_EXTRA_SRCS ${STUBS}  ${STARTUP_SRCS})

# COMPONENT_ADD_INCLUDEDIRS: 引出到应用工程的include目录
set(COMPONENT_ADD_INCLUDEDIRS "include")

# COMPONENT_PRIV_INCLUDEDIRS: 编译本组件需要的内部头文件目录
# set(COMPONENT_PRIV_INCLUDEDIRS "")

# COMPONENT_REQUIRES: 本组件依赖的其他组件
set(COMPONENT_REQUIRES "${TARGET}")

register_component()
