#  SPDX-License-Identifier: MIT-0 OR CC0-1.0  #
#     Authored 2025 by Peter S. Hollander     #


# The (minimum) versions below are what the Pico SDK uses
cmake_minimum_required(VERSION 3.13...4.0.1)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-ffile-prefix-map=${CMAKE_SOURCE_DIR}=.)


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



#set(PICO_BOARD pico2)
#set(PICO_PLATFORM rp2350-riscv)



# Include the Pico SDK & Extras (build functions, compiling definitions, etc)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{PICO_EXTRAS_PATH}/external/pico_extras_import.cmake)


PROJECT(index-audio-interface C CXX ASM)


set(SKIP_PICO_AUDIO_I2S TRUE)  # Skips pico_audio_i2s within pico-extras

# Invokes the Pico SDK's CMakeLists.txt
pico_sdk_init()

if (PICO_SDK_VERSION_STRING VERSION_LESS "2.0.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 2.0.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}.")
endif()

# Forcibly redefine the Pico SDK as INTERFACE_SYSTEM to suppress header warnings
set_target_properties(pico_base PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:pico_base,INTERFACE_INCLUDE_DIRECTORIES>)
set_target_properties(usb_device PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:usb_device,INTERFACE_INCLUDE_DIRECTORIES>)
set_target_properties(tinyusb_common_base PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:tinyusb_common_base,INTERFACE_INCLUDE_DIRECTORIES>)

# Generate a list of all program files we want to receive extra warnings for
set(EXTRA_COMPILER_WARNINGS "-Wall -Wextra -Wpedantic")
file(GLOB_RECURSE EXTRA_WARN_DIRECTORIES CONFIGURE_DEPENDS
    *.h
    *.hpp
    *.c
    *.cpp
)



add_subdirectory(uac1)
add_subdirectory(uac2)
