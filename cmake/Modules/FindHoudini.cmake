# - Try to find Houdini
#
#  This module defines the following variables
#
#  Houdini_FOUND - Was houdini found
#  Houdini_INCLUDE_DIRS - The houdini include directories
#  Houdini_LIBRARIES - Contains all the Houdini libraries
#  Houdini_FLAGS - Compiler definitions obtained from hcustom
#  Houdini_DSO_FLAGS - Additional flags specific to DSOs
#
#  This module accepts the following variables
#
#  Houdini_ROOT - Can be set to Houdini install path or Windows build path
#  HFS - Can be set to Houdini install path or Windows build path
#

# NOTE: This file has only been tested under Linux.

if( CMAKE_COMPILER_IS_GNUXX )
	message( FATAL_ERROR "Could not find GXX" )
endif()

set(Houdini_FOUND FALSE)

if ("$ENV{HFS}" STREQUAL "")
	#message( WARNING "$HFS environment does not exist. Make sure that you source the houdini_setup script!") 
else()
	set(Houdini_FOUND TRUE)
endif()

if(Houdini_FOUND)
	#Make sure we can locate the houdini binary
	find_program(Houdini houdini)

	message(STATUS "houdini var: ${Houdini}")
	if ( "${Houdini}" STREQUAL "Houdini-NOTFOUND" )
		message( FATAL_ERROR "Could not find houdini binary. Make sure that you source the houdini_setup script!" )
	endif()


	if( APPLE )
		set(_LIBDIR "$ENV{HFS}/../Libraries")
		set(_LIB_EXT dylib)
	elseif( UNIX )
		set(_LIBDIR "$ENV{HFS}/dsolib")
		set(_LIB_EXT so)
	elseif( WIN32 )
		set(_LIBDIR "$ENV{HFS}/dsolib")
		set(_LIB_EXT dll)
	endif()
	
	message(STATUS "libdir: ${_LIBDIR}")
	
	macro(_FIND_Houdini_LIBRARY _var)
		message(STATUS "Find Lib: ${_var} and argn: ${ARGN} with hfs $ENV{HFS}")
	  find_library(_var
	     NAMES 
	        ${ARGN}
			 PATHS $ENV{HFS} ENV HFS
			 PATH_SUFFIXES ../Libraries ./dsolib
	  )
		message(STATUS "find lib result : ${_var}")
	  mark_as_advanced(${_var})
	endmacro()
	
	set(Houdini_INCLUDE_DIRS "$ENV{HFS}/toolkit/include;$ENV{HFS}/toolkit/include/htools")
	
	if (APPLE)
		set (Houdini_LIBRARY_PATH "$ENV{HFS}/../Libraries")
	else()
		set( Houdini_LIBRARY_PATH "$ENV{HFS}/dsolib" )
	endif()
	
	set(Houdini_LIBRARIES
	HoudiniAPPS1
	HoudiniAPPS2
	HoudiniAPPS3
	HoudiniDEVICE
	HoudiniGEO
	HoudiniOP1
	HoudiniOP2
	HoudiniOP3
	HoudiniOPZ
	HoudiniPRM
	HoudiniRAY
	HoudiniSIM
	HoudiniUI
	HoudiniUT )
	
	execute_process( COMMAND hcustom -c
		OUTPUT_VARIABLE Houdini_FLAGS 
		OUTPUT_STRIP_TRAILING_WHITESPACE
		)
	
	execute_process( COMMAND hcustom -m
		OUTPUT_VARIABLE _Houdini_LIBS 
		OUTPUT_STRIP_TRAILING_WHITESPACE
		)
	
	set( Houdini_DSO_FLAGS "-DMAKING_DSO" )
	
	if (APPLE)
		#TODO - These flags should be exported as a variable to allow the user to apply them selectively to install targets
		set (CMAKE_SHARED_LINKER_FLAGS " ${_Houdini_LIBS} " )
	elseif (UNIX)
		#Clean the output from hcustom
		string( REGEX MATCHALL "-[l]([a-zA-Z0-9]+)" _CLEANLIBS ${_Houdini_LIBS} )
		string( REGEX REPLACE "-[lL]" ";" _CLEANLIBS ${_CLEANLIBS} )
		message("houdini libs: ${Houdini_LIBRARIES}")
		message("clean libs: ${_CLEANLIBS}")
		set( Houdini_LIBRARIES "${Houdini_LIBRARIES} ${_CLEANLIBS}" )
	endif()
endif(Houdini_FOUND)
