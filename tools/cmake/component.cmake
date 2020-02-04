# 泛海数据Sensor Development Framework
# 初始创意来自ESP-IDF

cmake_minimum_required(VERSION 3.5)

# 将库组件加到CMake环境
# 递归添加所有依赖的组件(每个组件目录只加一次)
# 使用全局变量ALL_COMPONENTS_ADDED
function(add_component_subdirectory component)
    list(FIND ALL_COMPONENTS_ADDED ${component} found)
    if ("${found}" STREQUAL "-1" ) 
        add_subdirectory("$ENV{PHNX_SDF}/components/${component}" "${PROJECT_PATH}/build/components/${component}")
        list(APPEND ALL_COMPONENTS_ADDED ${component})
        set(ALL_COMPONENTS_ADDED ${ALL_COMPONENTS_ADDED} CACHE STRING "ALL_COMPONENTS_ADDED" FORCE) 
        foreach(elm ${COMPONENT_${component}_REQUIRES})
            add_component_subdirectory(${elm})
            set(ALL_COMPONENTS_ADDED ${ALL_COMPONENTS_ADDED} CACHE STRING "ALL_COMPONENTS_ADDED" FORCE) 
        endforeach()
    endif()
endfunction()

# 生成库组件全部依赖列表(包含递归依赖)
# 以COMPONENT_${component}_ALL_REQUIRES形式存到CMake CACHE
function(set_component_all_requires component subcomponent)
    foreach(elm ${COMPONENT_${subcomponent}_REQUIRES})
        if(NOT "${elm}" STREQUAL "${component}")
            list(FIND COMPONENT_${component}_ALL_REQUIRES ${elm} found)
            if ("${found}" STREQUAL "-1" ) 
                list(APPEND COMPONENT_${component}_ALL_REQUIRES ${elm})
                set(COMPONENT_${component}_ALL_REQUIRES ${COMPONENT_${component}_ALL_REQUIRES} CACHE STRING "COMPONENT_${elm}_ALL_REQUIRES" FORCE)
                set_component_all_requires(${component} ${elm})
            endif()
        endif()
    endforeach()
endfunction()

# 注册组件
# 组件中可以定义：
# COMPONENT_SRCS : 组件源码
# COMPONENT_ADD_INCLUDEDIRS : 组件对外开放的头文件目录, 存入CMake CACHE COMPONENT_${elm}_INCLUDE
# COMPONENT_PRIV_INCLUDEDIRS : 组件内部使用的头文件目录, 存入CMake CACHE COMPONENT_${elm}_PRIV_INCLUDE
# COMPONENT_REQUIRES : 组件依赖的其他组件, 存入CMake CACHE  COMPONENT_${elm}_REQUIRES
function(register_component)
    get_filename_component(component_dir ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)
    get_filename_component(COMPONENT ${component_dir} NAME)
    set(COMPONENT ${COMPONENT} PARENT_SCOPE)
    spaces2list(COMPONENT_SRCS)
    spaces2list(COMPONENT_ADD_INCLUDEDIRS)
    spaces2list(COMPONENT_PRIV_INCLUDEDIRS)
    spaces2list(COMPONENT_REQUIRES)

    add_library(${COMPONENT} STATIC ${COMPONENT_SRCS})
    
    set(LIB_INCLUDE "")
	foreach(elm ${COMPONENT_ADD_INCLUDEDIRS})
		get_filename_component(temp ${elm} ABSOLUTE)
        list(APPEND LIB_INCLUDE ${temp})
	endforeach()
    set(COMPONENT_${elm}_INCLUDE ${LIB_INCLUDE} CACHE STRING "COMPONENT_${elm}_INCLUDE")

    set(LIB_INCLUDE "")
	foreach(elm ${COMPONENT_PRIV_INCLUDEDIRS})
		get_filename_component(temp ${elm} ABSOLUTE)
        list(APPEND LIB_INCLUDE ${temp})
	endforeach()
    set(COMPONENT_${elm}_PRIV_INCLUDE ${LIB_INCLUDE} CACHE STRING "COMPONENT_${elm}_PRIV_INCLUDE") 

    set(COMPONENT_${elm}_REQUIRES ${COMPONENT_REQUIRES} CACHE STRING "COMPONENT_${elm}_REQUIRES")    
endfunction()