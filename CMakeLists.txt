#
# General Container
#
cmake_minimum_required(VERSION 3.5)
project(ccntr)

add_subdirectory(lib)

find_library(HAVE_CMOCKA cmocka)
message("cmocka: ${HAVE_CMOCKA}")
if(HAVE_CMOCKA)
    add_subdirectory(test)
endif()
