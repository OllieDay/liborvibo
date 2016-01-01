cmake_minimum_required(VERSION 3.0)

project(orvibo C)
set(MAKE_BUILD_TYPE Debug)

file(GLOB SOURCES src/*.c)

add_library(shared SHARED ${SOURCES})
add_library(static STATIC ${SOURCES})

find_package(Threads REQUIRED)
target_link_libraries(shared ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(static ${CMAKE_THREAD_LIBS_INIT})

set_target_properties(shared PROPERTIES OUTPUT_NAME orvibo CLEAN_DIRECT_OUTPUT 1)
set_target_properties(static PROPERTIES OUTPUT_NAME orvibo CLEAN_DIRECT_OUTPUT 1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g -pedantic-errors -Wno-padded -Werror")

if (CMAKE_C_COMPILER_ID MATCHES "Clang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Weverything")
elseif (CMAKE_C_COMPILER_ID MATCHES "GNU")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
endif()

install(FILES include/orvibo.h DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
install(TARGETS shared LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")
