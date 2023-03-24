
set( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" )

list( APPEND SOURCES
      "${DIRECTORY}/Message.cpp"
      "${DIRECTORY}/MessageProcessor.cpp"
      "${DIRECTORY}/MessageQueue.cpp"
      "${DIRECTORY}/TestMessage.cpp"
    )

set( SOURCES "${SOURCES}" PARENT_SCOPE )