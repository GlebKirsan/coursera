cmake_minimum_required(VERSION 3.0)

project(cpp_yandex)


if(WIN32)
	set(PLATFORM "win")
elseif(UNIX)
	set(PLATFORM "unix")
endif()

message("Platform is ${PLATFORM}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") # -Wall -Werror -Wpedantic -Wextra
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "release")
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CONFIGURATION "debug")
else()
	set(CONFIGURATION "release")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O4")
endif()
message("Configuration is ${CONFIGURATION}")
message("Compiler flasgs: ${CMAKE_CXX_FLAGS}")

set(PREFIX "${CMAKE_SOURCE_DIR}/bin.${PLATFORM}")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PREFIX}.${CONFIGURATION}")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PREFIX}.debug")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PREFIX}.release")
set(CMAKE_CXX_STANDARD 17)


add_subdirectory(1_white_belt)
add_subdirectory(2_yellow_belt)
add_subdirectory(3_red_belt)
#add_subdirectory(4_brown_belt)
#add_subdirectory(5_black_belt)
