#  SPDX-License-Identifier: MIT-0 OR CC0-1.0  #
#     Authored 2025 by Peter S. Hollander     #


PROJECT(pico_usb_i2s_speaker C CXX ASM)


add_subdirectory(lib/pico-i2s-pio)



add_executable(${PROJECT_NAME}
    src/main.c
    lib/tinyusb/usb_descriptors.c
)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    lib/tinyusb
    ../shared/lib
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
)

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_dma
    tinyusb_device
    tinyusb_board
    pico-i2s-pio
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



pico_set_program_name(${PROJECT_NAME}        "pico_usb_i2s_speaker")
pico_set_program_version(${PROJECT_NAME}     "0.1")

# Creates map/bin/hex/uf2 files, in addition to elf
pico_add_extra_outputs(${PROJECT_NAME})
