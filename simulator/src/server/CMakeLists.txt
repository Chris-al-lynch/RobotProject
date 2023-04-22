
set( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" )

list( APPEND SOURCES
      "${DIRECTORY}/Server.cpp"
      "${DIRECTORY}/TcpServer.cpp"
    )

set( SOURCES "${SOURCES}" PARENT_SCOPE )