cmake_minimum_required(VERSION 3.10)

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

file(GLOB CHILD_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/external ${CMAKE_CURRENT_SOURCE_DIR}/external/*)

foreach(EXTERNAL_LIB ${CHILD_DIRS})
    if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/external/${EXTERNAL_LIB}")
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/${EXTERNAL_LIB})
        
        if(TARGET ${EXTERNAL_LIB})
            target_link_libraries(engine PUBLIC ${EXTERNAL_LIB})
        endif()

    endif()
endforeach()

file(GLOB_RECURSE ALL_MODULE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/modules/*.c)

add_library(modules STATIC ${ALL_MODULE_SRC})

target_include_directories(modules PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/core/include
    ${CMAKE_CURRENT_SOURCE_DIR}/modules
)

target_link_libraries(modules PUBLIC engine)