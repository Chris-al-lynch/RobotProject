# Beginner cmake file.  Will enhance once I get started on this component.
cmake_minimum_required( VERSION 3.26 )

project( RobotSimulator
         VERSION 0.1
         DESCRIPTION "Robot Simulator"
         LANGUAGES CXX )

set( BIN_DIR bin )
set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED True )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR} )
set( CPP_VERSION 11 )
set( CMAKE_BUILD_TYPE Debug )

include_directories( /usr/include /usr/include/c++/${CPP_VERSION}
                     src/message src/robot src/robot/movement src/server
                     src/utilities )

set( SOURCES )

add_subdirectory( "src" )
add_subdirectory( "src/message" )
add_subdirectory( "src/robot" )
add_subdirectory( "src/server" )
add_subdirectory( "src/utilities" )

message( "Sources = ${SOURCES}" )

add_executable( robotSimulator ${SOURCES} )

