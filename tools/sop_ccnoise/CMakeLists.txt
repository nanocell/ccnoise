

find_package( Boost 1.46 REQUIRED COMPONENTS random )
find_package( Houdini REQUIRED )

include_directories(${HOUDINI_INCLUDE_DIR})

add_library(sop_ccnoise sop_ccnoise/sop_ccnoise.cpp)
target_link_libraries(sop_ccnoise ${HOUDINI_LIBARIES} )
set_target_properties(sop_ccnoise PROPERTIES COMPILE_FLAGS ${HOUDINI_FLAGS})

#install( TARGETS gentable DESTINATION bin/gentable )

