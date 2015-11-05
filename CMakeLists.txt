cmake_minimum_required(VERSION 3.0)

project(orvibo C)
set(MAKE_BUILD_TYPE Debug)

file(GLOB SOURCES "src/*.c")

add_library(shared SHARED ${SOURCES})
add_library(static STATIC ${SOURCES})

find_package(Threads REQUIRED)
target_link_libraries(shared ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(static ${CMAKE_THREAD_LIBS_INIT})

set_target_properties(shared PROPERTIES OUTPUT_NAME orvibo)
set_target_properties(static PROPERTIES OUTPUT_NAME orvibo)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
