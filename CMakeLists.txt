cmake_minimum_required(VERSION 3.24.0)

project(setup LANGUAGES C CXX)
set (CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} UPPERCASE_CONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${UPPERCASE_CONFIG} ${CMAKE_SOURCE_DIR})
endforeach()

set(SOURCES
src/main.cpp
src/options.h
src/options.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC include)