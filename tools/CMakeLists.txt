

find_package( Boost 1.46 REQUIRED COMPONENTS random )
find_package( Houdini REQUIRED )

message(STATUS "Houdini Found: ${Houdini_FOUND}")

if(Houdini_FOUND)
	message(STATUS "Houdini includes: ${Houdini_INCLUDE_DIRS}")
	message(STATUS "Houdini libraries: ${Houdini_LIBRARIES}")
	
	include_directories(${Houdini_INCLUDE_DIRS})
	
	message("final libs: ${Houdini_LIBRARIES}")
	
	add_library(sop_ccnoise SHARED sop_ccnoise/sop_ccnoise.cpp)
	target_link_libraries(sop_ccnoise ${Houdini_LIBARIES} )
	
	set_target_properties(sop_ccnoise 
		PROPERTIES 
		COMPILE_FLAGS "${Houdini_FLAGS} ${Houdini_DSO_FLAGS}"
		PREFIX ""
		)

	#install( TARGETS gentable DESTINATION bin/gentable )
endif(Houdini_FOUND)
