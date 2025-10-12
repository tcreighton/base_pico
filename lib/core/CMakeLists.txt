# Create a library for drivers

add_library(core
    assertion.hpp
    board-config.hpp
    board-config.cpp
    error-context.hpp
    errors.hpp
    errors.cpp
    gpio.cpp
    gpio.hpp
    gpio-declarations.hpp
    logger.hpp
    logger.cpp
    packed-datetime.hpp
    packed-datetime.cpp
    product-info.hpp
    pystring.h
    pystring.cpp
    random.hpp
    serial-comm.hpp
    serial-comm.cpp
    utilities.hpp
    utilities.cpp
)


target_include_directories(core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(core PUBLIC
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
        pico_unique_id
)
