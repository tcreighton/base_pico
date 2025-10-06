
# Create a library for devices

add_library(devices
    component.hpp
    csi2c.cpp
    csi2c.hpp
    dac-declarations.hpp
    devicesContainer.hpp
    mcp-24lc32.cpp
    mcp-24lc32.hpp
    mcp-eeprom-declarations.hpp
    mcp4728.cpp
    mcp4728.hpp
)

target_include_directories(devices PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link against Pico SDK libraries this module needs
target_link_libraries(devices PUBLIC
        core
# pico interfaces here
#       hardware_adc
#        hardware_gpio
#        hardware_i2c
#       hardware_pwm
#       hardware_spi
#       hardware_timer
#       hardware_uart
#       pico_multicore
#       pico_stdlib
#       pico_sync
)
