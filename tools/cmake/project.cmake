# 泛海数据Sensor Development Framework
# 初始创意来自ESP-IDF
cmake_minimum_required(VERSION 3.5)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_MODULE_PATH})
include(component)

# 重定义 project().
# CMake 重定义时, 原函数仍可以下划线方式访问。
# https://cmake.org/pipermail/cmake/2015-October/061751.html.
function(project)
endfunction()

function(_project)
endfunction()

# 空格分隔转列表
function(spaces2list variable_name)
    if(${variable_name})
        string(REPLACE " " ";" tmp "${${variable_name}}")
        set("${variable_name}" "${tmp}" PARENT_SCOPE)
    else()
        set("${variable_name}" "" PARENT_SCOPE)
    endif()
endfunction()

# 重定义project
macro(project name)
    if(NOT TARGET)
        message(FATAL_ERROR "Please set TARGET to any supported platform in your CMakeList.txt file!")
    endif()
    # include 工具链定义
    include(toolchain-${TARGET})

    # 工程指定的编译参数定义
    if(PROJECT_DEFINITIONS)
        spaces2list(PROJECT_DEFINITIONS)
        add_compile_definitions(${PROJECT_DEFINITIONS})
    endif()

    if (TARGET_ENABLE_CXX)
        __project(${name} C CXX ASM)
    else()
        __project(${name} C ASM)
    endif()
    set(PROJECT_PATH "${CMAKE_SOURCE_DIR}")
    set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

    # 先添加全部依赖/组件目录
    if(PROJECT_REQUIRES)
        spaces2list(PROJECT_REQUIRES)
        list(APPEND PROJECT_REQUIRES "${TARGET}")
        list(REMOVE_DUPLICATES PROJECT_REQUIRES)
    elseif(RAW_PROJECT)
        # RAW工程不需要链接TARGET库
    else()
        set(PROJECT_REQUIRES "${TARGET}")
    endif()
    set(ALL_COMPONENTS_ADDED "" CACHE STRING "ALL_COMPONENTS_ADDED" FORCE)
    foreach(elm ${PROJECT_REQUIRES})
        add_component_subdirectory(${elm})
    endforeach()

    # 为组件添加所有依赖组件的头文件目录
    foreach(elm ${ALL_COMPONENTS_ADDED})
        target_include_directories(${elm} PRIVATE "${PROJECT_PATH}" "${PROJECT_INCLUDEDIRS}"
            ${COMPONENT_${elm}_PRIV_INCLUDE} ${COMPONENT_${elm}_INCLUDE})
        set(COMPONENT_${elm}_ALL_REQUIRES "" CACHE STRING "COMPONENT_${elm}_ALL_REQUIRES" FORCE)
        set_component_all_requires(${elm} ${elm})
        foreach(elm1 ${COMPONENT_${elm}_ALL_REQUIRES})
            list(APPEND component_inc ${COMPONENT_${elm1}_INCLUDE} )
            target_include_directories(${elm} PRIVATE ${COMPONENT_${elm1}_INCLUDE} )
        endforeach()
    endforeach()

    # 设置工程主文件编译
    set(PROJECT_EXECUTABLE ${CMAKE_PROJECT_NAME}.elf)

    set(REAL_PROJECT_SRCS "")
    if(PROJECT_SRCS)
        spaces2list(PROJECT_SRCS)
        foreach(elm ${ALL_COMPONENTS_ADDED})
            if(COMPONENT_${elm}_EXTRA_SRCS)
                list(APPEND PROJECT_SRCS ${COMPONENT_${elm}_EXTRA_SRCS} )
            endif()
        endforeach()
        # 移除CMakeCCompilerId.c, 便于在工程中使用 *.c作文件列表
        foreach(f ${PROJECT_SRCS})
            if(NOT f MATCHES "CMakeCCompilerId.c")
                list(APPEND REAL_PROJECT_SRCS ${f})
            endif()
        endforeach()
        add_executable(${PROJECT_EXECUTABLE} ${REAL_PROJECT_SRCS})
    else()
        message(FATAL_ERROR "PROJECT_SRCS shall be set in CMakeList.txt")
    endif()

    # 1. 头文件目录
    target_include_directories(${PROJECT_EXECUTABLE} PRIVATE ".")
    if(PROJECT_INCLUDEDIRS)
        spaces2list(PROJECT_INCLUDEDIRS)
        foreach(f ${PROJECT_INCLUDEDIRS})
            get_filename_component(temp ${f} ABSOLUTE)
            target_include_directories(${PROJECT_EXECUTABLE} PRIVATE ${temp})
        endforeach()
    endif()
    foreach(elm ${ALL_COMPONENTS_ADDED})
        target_include_directories(${PROJECT_EXECUTABLE} PRIVATE ${COMPONENT_${elm}_INCLUDE})
    endforeach()

    # 2. MAP 文件
    set(PROJECT_MAP ${CMAKE_PROJECT_NAME}.map)
    # set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

    target_link_libraries(${PROJECT_EXECUTABLE} "-Wl,--cref -Wl,--Map=${PROJECT_MAP}")
    set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" APPEND PROPERTY
        ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_MAP}")

    # 3. 库文件
    target_link_directories(${PROJECT_EXECUTABLE} PUBLIC "${LIBRARY_OUTPUT_PATH}")
    target_link_libraries(${PROJECT_EXECUTABLE} "-Wl,--start-group" ${PROJECT_REQUIRES} "-Wl,--end-group")
    # 如果工程指定了PROJECT_LDSCRIPT则不使用TARGET库中的ldscript
    if(PROJECT_LDSCRIPT)
        target_link_directories(${PROJECT_EXECUTABLE} PUBLIC ".")
        target_link_libraries(${PROJECT_EXECUTABLE} "-T ${PROJECT_LDSCRIPT}")
    endif()


    # 4. 将ELF文件转成BIN
    set(PROJECT_BIN ${CMAKE_PROJECT_NAME}.bin)
    add_custom_command(TARGET ${PROJECT_EXECUTABLE} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Obinary -S ${PROJECT_EXECUTABLE} ${PROJECT_BIN}
        COMMAND ${CMAKE_SIZE} --format=berkeley ${PROJECT_EXECUTABLE}
        COMMENT "Building ${PROJECT_BIN}"
    )

    # 5. 生成工程表述JSON文件，便于其他工具获取工程信息
    get_filename_component(PHNX_SDF $ENV{PHNX_SDF} ABSOLUTE)
    configure_file("$ENV{PHNX_SDF}/components/${TARGET}/.project/project_description.json.in"
        "project_description.json" @ONLY)
    configure_file("$ENV{PHNX_SDF}/components/${TARGET}/.project/vscode/c_cpp_properties.json.in"
        "${CMAKE_SOURCE_DIR}/.vscode/c_cpp_properties.json" @ONLY)
    configure_file("$ENV{PHNX_SDF}/components/${TARGET}/.project/vscode/extensions.json.in"
        "${CMAKE_SOURCE_DIR}/.vscode/extensions.json" @ONLY)
    configure_file("$ENV{PHNX_SDF}/components/${TARGET}/.project/vscode/launch.json.in"
        "${CMAKE_SOURCE_DIR}/.vscode/launch.json" @ONLY)
    configure_file("$ENV{PHNX_SDF}/components/${TARGET}/.project/vscode/tasks.json.in"
        "${CMAKE_SOURCE_DIR}/.vscode/tasks.json" @ONLY)

    function(project)
        set(project_ARGV ARGV)
        __project(${${project_ARGV}})
    endfunction()

endmacro()
