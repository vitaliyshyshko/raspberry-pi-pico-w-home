cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(home C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(home home.c
        lib/display.c
)

pico_enable_stdio_usb(home 1)
pico_enable_stdio_uart(home 1)

pico_add_extra_outputs(home)

target_compile_definitions(home PRIVATE
        WIFI_SSID=\"${WIFI_SSID}\"
        WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
)

target_include_directories(home PRIVATE ${CMAKE_CURRENT_LIST_DIR} )

target_link_libraries(
        home
        pico_cyw43_arch_lwip_threadsafe_background
        pico_stdlib
        hardware_adc
        hardware_i2c
)