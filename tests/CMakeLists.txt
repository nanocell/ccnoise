

find_package( Boost 1.46 REQUIRED COMPONENTS random )
find_package( CppUnit REQUIRED )

add_executable(test_vnoise test_vnoise.cpp )

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


