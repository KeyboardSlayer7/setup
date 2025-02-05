cmake_minimum_required(VERSION 3.24.0)

project(setup LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY /bin)

set(SOURCES
src/main.cpp
src/options.h
src/options.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})