
# Create a library for drivers

add_library(drivers
    board-config.hpp
    board-init.hpp
    eeprom0.cpp
    eeprom0.hpp
    eeprom0-page-declarations.hpp
    gpio.cpp
    gpio.hpp
    gpio-declarations.hpp
    driversContainer.hpp
)

target_include_directories(drivers PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link against Pico SDK libraries this module needs
target_link_libraries(drivers PUBLIC
        core
        devices
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
)
