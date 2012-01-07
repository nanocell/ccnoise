
cmake_minimum_required( VERSION 2.8 )

project(ccnoise)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include_directories( ./include )

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(tools)


