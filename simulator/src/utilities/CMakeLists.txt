
set( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" )

list( APPEND SOURCES
      "${DIRECTORY}/Logger.cpp"
      "${DIRECTORY}/utils.cpp"
    )

set( SOURCES "${SOURCES}" PARENT_SCOPE )