cmake_minimum_required(VERSION 3.10)

add_subdirectory(external/glad)
add_subdirectory(external/glfw)
add_subdirectory(external/miniaudio)

project(TRIO C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)

add_library(engine STATIC
    core/src/audio.c
    core/src/graphics.c
    core/src/input.c
    core/src/log.c
    core/src/window.c
)

target_include_directories(engine PUBLIC core/include)
target_link_libraries(engine PUBLIC
    glad
    glfw
    miniaudio
)

file(GLOB_RECURSE ALL_MODULE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/modules/*.c)

message(STATUS "Module sources: ${ALL_MODULE_SRC}")

add_library(modules STATIC ${ALL_MODULE_SRC})

target_include_directories(modules PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/core/include
    ${CMAKE_CURRENT_SOURCE_DIR}/modules
)

target_link_libraries(modules PUBLIC engine)