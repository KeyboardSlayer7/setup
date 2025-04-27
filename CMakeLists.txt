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

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

set(SOURCES
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/options.h
    ${SOURCE_DIR}/options.cpp
    ${SOURCE_DIR}/directories.h
    ${SOURCE_DIR}/directories.cpp
    ${SOURCE_DIR}/commands.h
    ${SOURCE_DIR}/commands.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${json_SOURCE_DIR}/single_include)

set(FILES_TO_COPY ${CMAKE_SOURCE_DIR}/options.json ${CMAKE_SOURCE_DIR}/cmake.template)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${FILES_TO_COPY}
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}
)
