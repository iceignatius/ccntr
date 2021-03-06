#
# Container Library
#
cmake_minimum_required(VERSION 3.5)
project(ccntr_library)

set(srcs ${srcs} ${CMAKE_SOURCE_DIR}/src/ccntr_spinlock.c)
set(srcs ${srcs} ${CMAKE_SOURCE_DIR}/src/ccntr_man_array.c)
set(srcs ${srcs} ${CMAKE_SOURCE_DIR}/src/ccntr_list.c)
set(srcs ${srcs} ${CMAKE_SOURCE_DIR}/src/ccntr_man_list.c)
set(srcs ${srcs} ${CMAKE_SOURCE_DIR}/src/ccntr_queue.c)
set(srcs ${srcs} ${CMAKE_SOURCE_DIR}/src/ccntr_man_queue.c)
set(srcs ${srcs} ${CMAKE_SOURCE_DIR}/src/ccntr_stack.c)
set(srcs ${srcs} ${CMAKE_SOURCE_DIR}/src/ccntr_man_stack.c)
set(srcs ${srcs} ${CMAKE_SOURCE_DIR}/src/ccntr_map.c)
set(srcs ${srcs} ${CMAKE_SOURCE_DIR}/src/ccntr_man_map.c)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_BINARY_DIR})

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
endif()
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

add_library(ccntr ${srcs})

install(TARGETS ccntr DESTINATION lib)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include/ccntr)
