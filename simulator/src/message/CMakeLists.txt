
set( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" )

list( APPEND SOURCES
      "${DIRECTORY}/Message.cpp"
      "${DIRECTORY}/MessageFactory.cpp"
      "${DIRECTORY}/MessageHeader.cpp"
      "${DIRECTORY}/MessageProcessor.cpp"
      "${DIRECTORY}/MessageQueue.cpp"
      "${DIRECTORY}/MessageTrailer.cpp"
      "${DIRECTORY}/MoveMessage.cpp"
      "${DIRECTORY}/MoveMessageResponse.cpp"
      "${DIRECTORY}/Response.cpp"
      "${DIRECTORY}/ResponseQueue.cpp"
      "${DIRECTORY}/TestMessage.cpp"
      "${DIRECTORY}/TestMessageResponse.cpp"
    )

set( SOURCES "${SOURCES}" PARENT_SCOPE )