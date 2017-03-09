#
# General Container
#
cmake_minimum_required(VERSION 3.5)
project(ccntr)

include(CheckFunctionExists)
check_function_exists(malloc HAVE_MALLOC)
check_function_exists(free HAVE_FREE)

configure_file("${CMAKE_SOURCE_DIR}/config.h.in"
               "${CMAKE_BINARY_DIR}/config.h")

add_subdirectory(lib)

find_library(HAVE_CMOCKA cmocka)
message("cmocka: ${HAVE_CMOCKA}")
if(HAVE_CMOCKA)
    add_subdirectory(test)
endif()
