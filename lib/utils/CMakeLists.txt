
add_library(utils
        logger.cpp
        packed-datetime.cpp
        pystring.h
        utilities.cpp
)

target_include_directories(utils PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(utils PUBLIC

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