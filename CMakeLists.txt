#
# General Container
#
cmake_minimum_required(VERSION 3.5)
project(ccntr)

include(CheckFunctionExists)
check_function_exists(malloc CCNTR_HAVE_MALLOC)
check_function_exists(free CCNTR_HAVE_FREE)

option(CCNTR_THREAD_SAFE "Thread safe mode" OFF)

configure_file("${CMAKE_SOURCE_DIR}/ccntr_config.h.in"
               "${CMAKE_BINARY_DIR}/ccntr_config.h")

add_subdirectory(lib)

find_library(HAVE_CMOCKA cmocka)
message("cmocka: ${HAVE_CMOCKA}")
if(HAVE_CMOCKA)
    add_subdirectory(test)
endif()
