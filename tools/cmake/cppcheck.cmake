if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    find_program(CMAKE_C_CPPCHECK NAMES cppcheck)
    if (CMAKE_CXX_CPPCHECK)
        list(
            APPEND CMAKE_C_CPPCHECK
                "--enable=warning"
                "--inconclusive"
                "--force"
                # "--inline-suppr"
                # "--suppressions-list=${CMAKE_SOURCE_DIR}/CppCheckSuppressions.txt"
        )
    endif()
endif()
