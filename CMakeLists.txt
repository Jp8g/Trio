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

file(GLOB MODULE_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/modules modules/*)

foreach(module ${MODULE_DIRS})
    file(GLOB_RECURSE MODULE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/modules/${module}/src/*.c)
    if(MODULE_SRC)
        add_library(trio_module_${module} STATIC ${MODULE_SRC})
        target_include_directories(trio_module_${module} PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/modules/${module}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/core/include
        )
        target_link_libraries(trio_module_${module} PUBLIC engine)
        list(APPEND ALL_MODULE_LIBS trio_module_${module})
    endif()
endforeach()

if(ALL_MODULE_LIBS)
    add_library(modules STATIC "")
    target_link_libraries(modules PUBLIC ${ALL_MODULE_LIBS})
endif()