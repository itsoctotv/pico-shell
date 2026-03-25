# Minimum CMake version
cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(boot)
pico_sdk_init()


# Add executable
include_directories(inc/)
include_directories(inc/littlefs/)
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.c)
add_executable(boot ${SRC_FILES})


# Link Pico SDK libraries
#target_link_libraries(boot pico_stdlib hardware_spi) #externalflash
target_link_libraries(boot pico_stdlib hardware_flash hardware_sync)

# Enable USB output, UART output, etc.
pico_enable_stdio_usb(boot 1)

# Create map/bin/hex files
pico_add_extra_outputs(boot)
