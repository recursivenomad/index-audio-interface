#  SPDX-License-Identifier: MIT-0 OR CC0-1.0  #
#     Authored 2025 by Peter S. Hollander     #


# The (minimum) versions below are what the Pico SDK uses
cmake_minimum_required(VERSION 3.13...4.0.1)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)



# Include the Pico SDK & Extras (build functions, compiling definitions, etc)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{PICO_EXTRAS_PATH}/external/pico_extras_import.cmake)


PROJECT(usb_sound_card C CXX ASM)


# Invokes the Pico SDK's CMakeLists.txt
pico_sdk_init()

if (PICO_SDK_VERSION_STRING VERSION_LESS "2.0.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 2.0.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}.")
endif()



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
    PICO_USBDEV_ENABLE_DEBUG_TRAgCE
    PICO_AUDIO_I2S_MONO_OUTPUT=0
    PICO_AUDIO_I2S_MONO_INPUT=0
)

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    usb_device
    pico_audio_i2s
    pico_multicore
)



# Creates map/bin/hex/uf2 files, in addition to elf
pico_add_extra_outputs(${PROJECT_NAME})
