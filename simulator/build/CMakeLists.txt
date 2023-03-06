# Beginner cmake file.  Will enhance once I get started on this component.
cmake_minimum_required( VERSION 3.26 )

project( RobotSimulator
         VERSION 0.1
         DESCRIPTION "Robot Simulator"
         LANGUAGES CXX )

set( BIN_DIR ../bin )
set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED True )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR} )

add_executable( robotSimulator ../src/Simulator.cpp )