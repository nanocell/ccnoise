
cmake_minimum_required( VERSION 2.8 )

project(ccnoise)

find_package( Boost 1.46 REQUIRED COMPONENTS random )

include_directories( ./include )

add_subdirectory(tools)
add_subdirectory(src)



