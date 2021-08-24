cmake_minimum_required(VERSION 3.0)

project(cpp_yandex)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") # -Wall -Werror -Wpedantic -Wextra
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "release")
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CONFIGURATION "debug")
else()
	set(CONFIGURATION "release")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()
message("Configuration is ${CONFIGURATION}")
message("Compiler flasgs: ${CMAKE_CXX_FLAGS}")

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(headers)

add_subdirectory(1_white_belt)
add_subdirectory(2_yellow_belt)
add_subdirectory(3_red_belt)
add_subdirectory(4_brown_belt)
#add_subdirectory(5_black_belt)