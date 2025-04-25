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



# Include the Pico SDK & Extras (build functions, compiling definitions, etc)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{PICO_EXTRAS_PATH}/external/pico_extras_import.cmake)


PROJECT(usb_sound_card C CXX ASM)


set(SKIP_PICO_AUDIO_I2S TRUE)  # Skips pico_audio_i2s within pico-extras

# Invokes the Pico SDK's CMakeLists.txt
pico_sdk_init()

if (PICO_SDK_VERSION_STRING VERSION_LESS "2.0.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 2.0.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}.")
endif()

add_subdirectory(lib/i2s)  # Use local i2s instead



add_executable(${PROJECT_NAME}
    src/main.c
)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    src/
    lib/
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    AUDIO_FREQ_MAX=48000
    PICO_USBDEV_MAX_DESCRIPTOR_SIZE=256
    PICO_USBDEV_USE_ZERO_BASED_INTERFACES=1
    PICO_USBDEV_ISOCHRONOUS_BUFFER_STRIDE_TYPE=1
)

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    usb_device
    pico_audio_i2s
    pico_multicore
)



# Creates map/bin/hex/uf2 files, in addition to elf
pico_add_extra_outputs(${PROJECT_NAME})
