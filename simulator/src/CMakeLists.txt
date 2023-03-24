
set( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" )

message( "src dir = ${DIRECTORY}" )

list( APPEND SOURCES "${DIRECTORY}/Main.cpp" )

set( SOURCES "${SOURCES}" PARENT_SCOPE )
