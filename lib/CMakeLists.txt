#
# Container Library
#
cmake_minimum_required(VERSION 3.5)
project(ccntr_library)

set(srcs ${srcs} ${CMAKE_SOURCE_DIR}/src/ccntr_list.c)
set(srcs ${srcs} ${CMAKE_SOURCE_DIR}/src/ccntr_man_list.c)

include_directories(${CMAKE_SOURCE_DIR}/include)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

add_library(ccntr ${srcs})
