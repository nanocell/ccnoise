

find_package( Boost 1.47 REQUIRED COMPONENTS date_time )
find_package( CppUnit REQUIRED )

message(STATUS "Boost found: ${Boost_FOUND}")

if (!Boost_FOUND)
	message(FATAL_ERROR "Could not find boost!")
endif(!Boost_FOUND)

message(STATUS "Boost includes: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost libraries: ${Boost_LIBRARIES}")

include_directories(${Boost_INCLUDE_DIRS} ${CppUnit_INCLUDE_DIRS})

add_executable(test_vnoise test_vnoise.cpp )
target_link_libraries(test_noise ${BOOST_LIBRARIES})

add_executable(test_spline test_spline.cpp )
target_link_libraries(test_spline ${CPPUNIT_LIBRARIES})

add_executable(test_clamp test_clamp.cpp )
target_link_libraries(test_clamp ${CPPUNIT_LIBRARIES})

add_executable(test_floor test_floor.cpp )
target_link_libraries(test_floor ${CPPUNIT_LIBRARIES})

add_executable(test_smoothstep test_smoothstep.cpp )
target_link_libraries(test_smoothstep ${CPPUNIT_LIBRARIES})

#This file is just for testing preprocessor expansion
# e.g. g++ -P -E -CC test_preprocessor.cpp
#add_executable(test_preprocessor test_preprocessor.cpp )
#target_link_libraries(test_preprocessor ${CPPUNIT_LIBRARIES})

#add_executable(test_vector test_vector.cpp Vector3.cpp Matrix33.cpp)
#target_link_libraries(test_vector ${CPPUNIT_LIBRARIES})

#install( TARGETS test_vnoise DESTINATION bin/test_vnoise )
#install( TARGETS test_spline DESTINATION bin/test_spline )
#install( TARGETS test_clamp DESTINATION bin/test_clamp )


