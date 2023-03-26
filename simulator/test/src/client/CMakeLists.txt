# Beginner cmake file.  Will enhance once I get started on this component.
cmake_minimum_required( VERSION 3.26 )

project( "RobotSimulator Test"
         VERSION 0.1
         DESCRIPTION "Robot Simulator"
         LANGUAGES CXX )

set( BIN_DIR bin )
set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED True )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR} )
set( CPP_VERSION 11 )

include_directories( /usr/include /usr/include/c++/${CPP_VERSION}
                    ../../../src/message ../../../src/utilities )
set( SRC_DIR ../../../src )
set( SOURCES ${SRC_DIR}/message/Message.cpp
             ${SRC_DIR}/message/TestMessage.cpp
             ${SRC_DIR}/utilities/Logger.cpp
             ${SRC_DIR}/utilities/utils.cpp
   )
set( TEST_SOURCES ${SOURCES} Client.cpp Main.cpp )

message( "Sources = ${TEST_SOURCES}" )

add_executable( robotSimulatorTest ${TEST_SOURCES} )