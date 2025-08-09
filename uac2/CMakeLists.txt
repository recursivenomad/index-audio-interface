#  SPDX-License-Identifier: MIT-0 OR CC0-1.0  #
#     Authored 2025 by Peter S. Hollander     #


# The (minimum) versions below are what the Pico SDK uses
cmake_minimum_required(VERSION 3.13...4.0.1)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


# SDK option to not optimize anything during debug builds
set(PICO_DEOPTIMIZED_DEBUG ON)
# SDK option to disable firmware access restrictions
set(PICO_NO_CMSE ON)
# SDK option to load into and run this application faster from RAM
set(PICO_COPY_TO_RAM OFF)
# SDK options on where to interface stdio
set(PICO_STDIO_UART ON)
set(PICO_STDIO_USB OFF)
set(PICO_STDIO_RTT OFF)
set(PICO_STDIO_SEMIHOSTING OFF)



# Include the Pico SDK (build functions, compiling definitions, etc)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)


PROJECT(pico_usb_i2s_speaker C CXX ASM)


# Invokes the Pico SDK's CMakeLists.txt
pico_sdk_init()

if (PICO_SDK_VERSION_STRING VERSION_LESS "2.0.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 2.0.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}.")
endif()

add_subdirectory(lib/pico-i2s-pio)



add_executable(${PROJECT_NAME}
    src/main.c
    lib/tinyusb/usb_descriptors.c
)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    lib/tinyusb
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
)

target_link_libraries(${PROJECT_NAME}
    tinyusb_device
    tinyusb_board
    pico-i2s-pio
)



pico_set_program_name(${PROJECT_NAME}        "pico_usb_i2s_speaker")
pico_set_program_version(${PROJECT_NAME}     "0.1")

# Creates map/bin/hex/uf2 files, in addition to elf
pico_add_extra_outputs(${PROJECT_NAME})
