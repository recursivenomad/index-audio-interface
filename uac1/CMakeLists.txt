#  SPDX-License-Identifier: MIT-0 OR CC0-1.0  #
#     Authored 2025 by Peter S. Hollander     #


PROJECT(usb_sound_card C CXX ASM)


set(SKIP_PICO_AUDIO_I2S TRUE)  # Skips pico_audio_i2s within pico-extras
add_subdirectory(lib/i2s)      # Use local i2s instead



add_executable(${PROJECT_NAME}
    src/main.c
)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
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
