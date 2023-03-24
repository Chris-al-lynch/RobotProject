
#include <climits>
#include <cstdlib>

#include "Exceptions.h"
#include "Message.h"
#include "TestMessage.h"

TestMessage::TestMessage( char *messageBuffer ) : Message( messageBuffer )
{
}

char *
TestMessage::generateTestMessage( string message )
{
    size_t messageLength = sizeof( msgHeader ) + sizeof( msgTrailer )
                         + message.size() + 1;

    if( messageLength > INT_MAX )
    {
        throw MessageException( "Message size cannot be larger than "
                              + to_string( INT_MAX )
                              + " bytes.  Your message is "
                              + to_string( messageLength )
                              + " bytes." );
    }

    char *newMessage = (char *)calloc( messageLength, sizeof( char ) );

    if( newMessage == NULL )
    {
        logger->logError( "Failed to allocate memory for test message" );
        throw bad_alloc();
    }

    msgHeader header;
    msgTrailer trailer;
    
    generateMessageHeader( &header, messageLength, TEST_MESSAGE );
    generateTrailer( &trailer );

    memcpy( newMessage, &header, sizeof( msgHeader ) );
    memcpy( &newMessage[sizeof( header )], message.c_str(), message.size() + 1 );
    memcpy( &newMessage[sizeof( header ) + message.size() + 1], &trailer,
            sizeof( msgTrailer ) );

    return newMessage;
}

char *
TestMessage::generateTestResponse( string responseMessage )
{
    responseMessage = "Response: " + responseMessage;

    size_t messageLength = sizeof( msgHeader ) + sizeof( msgTrailer )
                         + responseMessage.size() + 1;

    if( messageLength > INT_MAX )
    {
        throw MessageException( "Response Message size cannot be larger than "
                              + to_string( INT_MAX )
                              + " bytes.  Your response message is "
                              + to_string( messageLength )
                              + " bytes." );
    }

    char *response = (char *)calloc( messageLength, sizeof( char ) );

    if( response == NULL )
    {
        logger->logError( "Failed to allocate memory for test response." );
        throw bad_alloc();
    }

    msgHeader header;
    msgTrailer trailer;

    generateResponseHeader( &header, messageLength, TEST_MESSAGE, getId() );
    generateTrailer( &trailer );

    memcpy( response, &header, sizeof( msgHeader ) );
    memcpy( &response[sizeof( header )], responseMessage.c_str(),
             responseMessage.size() + 1 );
    memcpy( &response[sizeof( header ) + responseMessage.size() + 1], &trailer,
            sizeof( msgTrailer ) );

    return response;
}

char *
TestMessage::processMessage()
{
    char *message = ((testMessage *)(&messageBuffer[sizeof( msgHeader )]))->message;

    string responseMessage = "Received message: " + string( message );
    return generateTestResponse( responseMessage );
}
