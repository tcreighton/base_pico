
add_library(utils
        command-declarations.hpp
        command-handler.cpp
        command-handler.hpp
        commands.cpp
        commands.hpp
        communication.hpp
        communication.cpp
)

target_include_directories(utils PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(utils PUBLIC
        core
        # pico interfaces here
        #       hardware_adc
        hardware_gpio
        hardware_i2c
        #       hardware_pwm
        #       hardware_spi
        #       hardware_timer
        #       hardware_uart
        #       pico_multicore
        pico_stdlib
        pico_stdio_usb
        #       pico_sync
        #       pico_unique_id
)