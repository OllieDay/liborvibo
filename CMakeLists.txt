cmake_minimum_required(VERSION 3.0)

project(orvibo)
set(MAKE_BUILD_TYPE Debug)

file(GLOB SOURCES "src/*.c")

add_library(shared SHARED ${SOURCES})
add_library(static STATIC ${SOURCES})


set_target_properties(shared PROPERTIES OUTPUT_NAME orvibo)
set_target_properties(static PROPERTIES OUTPUT_NAME orvibo)
