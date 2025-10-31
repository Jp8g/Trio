cmake_minimum_required(VERSION 3.10)

add_subdirectory(external/glad)
add_subdirectory(external/glfw)
add_subdirectory(external/miniaudio)

project(TRIO C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)

add_library(engine STATIC
    src/audio.c
    src/graphics.c
    src/input.c
    src/log.c
    src/window.c
)

target_include_directories(engine PUBLIC include)
target_link_libraries(engine PUBLIC
    glad
    glfw
    miniaudio
)

add_executable(sandbox sandbox/main.cpp)
target_link_libraries(sandbox PRIVATE engine)