#  SPDX-License-Identifier: MIT-0 OR CC0-1.0  #
#     Authored 2025 by Peter S. Hollander     #


PROJECT(usb_sound_card C CXX ASM)


add_subdirectory(lib/i2s)      # Use local i2s instead



add_executable(${PROJECT_NAME}
    src/main.c
)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    lib/
    ../shared/lib
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



if(${EXTRA_NON_IMPORTED_COMPILER_WARNINGS})

    # Assert additional compiler warnings for files beneath this directory

    file(GLOB_RECURSE WARN_DIRECTORIES CONFIGURE_DEPENDS
        *
    )

    set_source_files_properties(${WARN_DIRECTORIES} PROPERTIES
        COMPILE_FLAGS "-Wall -Wextra -Wpedantic"
    )

endif()



# Creates map/bin/hex/uf2 files, in addition to elf
pico_add_extra_outputs(${PROJECT_NAME})
