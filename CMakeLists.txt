

# To use this lists file, set PICO_SDK_PATH to point to where the sdk is.
# Then run cmake. For example, from project directory: cmake -B ./build -S ./source
# Finally, from the ./build directory: make or ninja

# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.13...3.30.2)

#set(PROJECT_HOME /home/tom/dev/pico/my_project)
#set(PROJECT_HOME ${CMAKE_SOURCE_DIR})
#enable exceptions for the project
set(PICO_CXX_ENABLE_EXCEPTIONS 1)
#enable Real Time Type Information
set(PICO_CXX_ENABLE_RTTI 0)
#set(PICO_SDK_HOME ${P})
message("Pico home: ${PICO_SDK_PATH}")
include(pico_sdk_import.cmake)
#include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

project(base_pico C CXX ASM)	# CHANGE ME!

# Configure before sdk init!
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(PICO_DEOPTIMIZED_DEBUG 1)
	set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
	message("Building with asserts enabled.")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	message("Building with asserts disabled.")
endif()

pico_sdk_init()

message(STATUS "Starting configuration.")
include(CMakePrintHelpers)
cmake_policy(SET CMP0054 NEW)	# ensures that quoted strings inside if() are always treated as literal strings, not as variable names.

add_subdirectory(lib/core)
add_subdirectory(lib/drivers)
add_subdirectory(lib/utils)
add_subdirectory(src)

# Handle board revision via a single variable
set(BOARD_REV "A" CACHE STRING "Board revision (0, A, B)")
set_property(CACHE BOARD_REV PROPERTY STRINGS 0 A B)

if(BOARD_REV STREQUAL "0")
	target_compile_definitions(${PROJECT_NAME} PRIVATE BOARD_BASE_PICO_REV_0)
	message(STATUS "Building for pico_base Rev 0")
elseif(BOARD_REV STREQUAL "A")
	target_compile_definitions(${PROJECT_NAME} PRIVATE BOARD_BASE_PICO_REV_A)
	message(STATUS "Building for pico_base Rev A")
elseif(BOARD_REV STREQUAL "B")
	target_compile_definitions(${PROJECT_NAME} PRIVATE BOARD_BASE_PICO_REV_B)
	message(STATUS "Building for pico_base Rev B")
else()
	message(FATAL_ERROR "Unknown BOARD_REV: ${BOARD_REV}")
endif()

message(STATUS "\nAfter add_executable and targets\n")
cmake_print_variables(PROJECT_NAME)
cmake_print_variables(PROJECT_SOURCE_DIR)
cmake_print_variables(PROJECT_BINARY_DIR)
cmake_print_variables(CMAKE_CURRENT_SOURCE_DIR)
cmake_print_variables(CMAKE_BUILD_TYPE)
cmake_print_variables(CMAKE_CXX_FLAGS_RELEASE)
cmake_print_variables(CMAKE_CXX_FLAGS_DEBUG)
cmake_print_variables(CMAKE_CXX_FLAGS)

cmake_print_variables($ENV{PICO_SDK_PATH})

#Integrate with Intellisense
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)


# Create map/bin/hex/uf2 files
#pico_add_extra_outputs(${PROJECT_NAME})


# Enable usb output, disable uart output
#pico_enable_stdio_usb(${PROJECT_NAME} 1)
#pico_enable_stdio_uart(${PROJECT_NAME} 0)
