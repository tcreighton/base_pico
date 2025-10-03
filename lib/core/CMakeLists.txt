# Create a library for drivers

add_library(core
    logger.hpp
    logger.cpp
    packed-datetime.hpp
    packed-datetime.cpp
    pystring.h
    pystring.cpp
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
        #       pico_sync
        pico_unique_id
)
