#
# Container Test
#
cmake_minimum_required(VERSION 3.5)
project(ccntr_test)

find_library(HAVE_CMOCKA cmocka)
if(NOT HAVE_CMOCKA)
    message(FATAL_ERROR "Test program depend on CMOCKA library!")
endif()

set(srcs ${srcs} ${PROJECT_SOURCE_DIR}/test_list.c)
set(srcs ${srcs} ${PROJECT_SOURCE_DIR}/test_man_list.c)
set(srcs ${srcs} ${PROJECT_SOURCE_DIR}/main.c)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

set(depend_libs ${depend_libs} ccntr)
set(depend_libs ${depend_libs} cmocka)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

add_executable(ccntr_test ${srcs})
target_link_libraries(ccntr_test ${depend_libs})
