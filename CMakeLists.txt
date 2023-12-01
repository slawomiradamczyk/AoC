cmake_minimum_required(VERSION 3.8)

project(AoC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(Utils.cmake)

file(GLOB children LIST_DIRECTORIES true RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/*)
foreach(child ${children})
    if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${child})
        if(EXISTS "${PROJECT_SOURCE_DIR}/${child}/CMakeLists.txt")
            add_subdirectory(${child})
        endif()
    endif()
endforeach()