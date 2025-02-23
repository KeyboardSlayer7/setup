cmake_minimum_required(VERSION 3.24.0)
include(FetchContent)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/include.zip 
	URL_HASH SHA256=a22461d13119ac5c78f205d3df1db13403e58ce1bb1794edc9313677313f4a9d
)

FetchContent_MakeAvailable(json)

project(setup LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

set(SOURCES
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/options.h
    ${SOURCE_DIR}/options.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${json_SOURCE_DIR}/single_include)