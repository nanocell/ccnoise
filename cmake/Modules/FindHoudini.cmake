# - Try to find Houdini
#
#  This module defines the following variables
#
#  HOUDINI_FOUND - Was houdini found
#  HOUDINI_INCLUDE_DIR - The houdini include directories
#  HOUDINI_LIBRARIES - Contains all the Houdini libraries
#  HOUDINI_FLAGS - Compiler definitions obtained from hcustom
#  HOUDINI_DSO_FLAGS - Additional flags specific to DSOs
#
#  This module accepts the following variables
#
#  HOUDINI_ROOT - Can be set to Houdini install path or Windows build path
#  HFS - Can be set to Houdini install path or Windows build path
#

# NOTE: This file has only been tested under Linux.

if( CMAKE_COMPILER_IS_GNUXX )
	message( FATAL_ERROR "Could not find GXX" )
endif()

if ("$ENV{HFS}" STREQUAL "")
	message( FATAL_ERROR "$HFS environment does not exist. Make sure that you source the houdini_setup script!") 
endif()

#Try to find houdini binary
find_program(HOUDINI houdini)

message(STATUS "houdini var: ${HOUDINI}")
if ( "${HOUDINI}" STREQUAL "HOUDINI-NOTFOUND" )
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

macro(_FIND_HOUDINI_LIBRARY _var)
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

set(HOUDINI_INCLUDE_DIR "$ENV{HFS}/toolkit/include;$ENV{HFS}/toolkit/include/htools")

if (APPLE)
	set (HOUDINI_LIBRARY_PATH "$ENV{HFS}/../Libraries")
else()
	set( HOUDINI_LIBRARY_PATH "$ENV{HFS}/dsolib" )
endif()

set(HOUDINI_LIBRARIES
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
	OUTPUT_VARIABLE HOUDINI_FLAGS 
	OUTPUT_STRIP_TRAILING_WHITESPACE
	)

execute_process( COMMAND hcustom -m
	OUTPUT_VARIABLE _HOUDINI_LIBS 
	OUTPUT_STRIP_TRAILING_WHITESPACE
	)

set( HOUDINI_DSO_FLAGS "-DMAKING_DSO" )

if (APPLE)
	#TODO - These flags should be exported as a variable to allow the user to apply them selectively to install targets
	set (CMAKE_SHARED_LINKER_FLAGS " ${_HOUDINI_LIBS} " )
elseif (UNIX)
	#Clean the output from hcustom
	string( REGEX MATCHALL "-[l]([a-zA-Z0-9]+)" _CLEANLIBS ${_HOUDINI_LIBS} )
	string( REGEX REPLACE "-[lL]" ";" _CLEANLIBS ${_CLEANLIBS} )
	message("houdini libs: ${HOUDINI_LIBRARIES}")
	message("clean libs: ${_CLEANLIBS}")
	set( HOUDINI_LIBRARIES "${HOUDINI_LIBRARIES} ${_CLEANLIBS}" )
endif()

