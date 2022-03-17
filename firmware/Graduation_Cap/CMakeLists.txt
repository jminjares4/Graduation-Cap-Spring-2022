cmake_minimum_required(VERSION 3.19)

add_executable(blink
        src/main.c
)

target_link_libraries(blink pico_stdlib freertos)
pico_add_extra_outputs(blink)