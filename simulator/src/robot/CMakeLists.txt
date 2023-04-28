
set( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" )

list( APPEND SOURCES
      "${DIRECTORY}/Motor.cpp"
      "${DIRECTORY}/Robot.cpp"
    )

set( SOURCES "${SOURCES}" PARENT_SCOPE )