
#include <climits>
#include <cstdlib>

#include "Exceptions.h"
#include "Message.h"
#include "TestMessage.h"
#include "utils.h"

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

    if( newMessage == nullptr )
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
TestMessage::generateTestResponse( string responseMessage, int msgId )
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

    if( response == nullptr )
    {
        //logger->logError( "Failed to allocate memory for test response." );
        throw bad_alloc();
    }

    msgHeader header;
    msgTrailer trailer;

    generateResponseHeader( &header, messageLength, TEST_MESSAGE, msgId );
    generateTrailer( &trailer );

    memcpy( response, &header, sizeof( msgHeader ) );
    memcpy( &response[sizeof( msgHeader )], responseMessage.c_str(),
             responseMessage.length() + 1 );
    memcpy( &response[sizeof( msgHeader ) + responseMessage.length() + 1],
            &trailer, sizeof( msgTrailer ) );

    return response;
}

char *
TestMessage::processMessage()
{
    logger->logInfo( "Test Message Processor..." );

    if( isMessageValid() )
    {
        logger->logInfo( "Message is valid." );
    }

    logger->logInfo( "Sizeof( header ) = " + to_string( sizeof( msgHeader ) ) );

    msgHeader *header = (msgHeader *)messageBuffer;

    int strLength = header->msgLength - sizeof( msgHeader ) - sizeof( msgTrailer );

    char message[strLength];
    memcpy( message, &messageBuffer[sizeof( msgHeader )], strLength );

    logger->logInfo( "Message Processed: " + string( message ) );

    string responseMessage = "Received message: " + string( message );
    return generateTestResponse( responseMessage, getId() );
}

bool
TestMessage::operator!=( TestMessage &msg1 )
{
    bool notEqual = false;
    msgHeader *header     = (msgHeader *)messageBuffer;
    msgHeader *msg1Header = (msgHeader *)msg1.messageBuffer;

    /* Note, the IDs can be different, that's OK. */
    if( header->msgLength != msg1Header->msgLength )
    {
        logger->logError( "header msgLength = " + to_string( header->msgLength ) 
                        + ", header1 msgLength = " + to_string( msg1Header->msgLength ) );
        notEqual = true;
    }
    else if( header->id != msg1Header->id )
    {
        logger->logError( "header id = " + to_string( header->id )
                        + ", header1 id = " + to_string( msg1Header->id ) );
        notEqual = true;
    }
    else if( header->msgType != msg1Header->msgType )
    {
        logger->logError( "header msgType = " + to_string( header->msgType )
                        + ", header1 msgType = " + to_string( msg1Header->msgType ) );
        notEqual = true;
    }
    else if( header->marker != msg1Header->marker )
    {
        logger->logError( "header marker = " + intToHexString( header->marker )
                        + ", header1 marker = " + intToHexString( msg1Header->marker ) );
        notEqual = true;
    }
    else
    {
        msgTrailer *trailer = (msgTrailer *)&(messageBuffer[getLength() - sizeof(msgTrailer)]);
        msgTrailer *msg1Trailer = (msgTrailer *)&(msg1.messageBuffer[getLength() - sizeof(msgTrailer)]);

        if( trailer->marker != msg1Trailer->marker )
        {
            logger->logError( "trailer marker = " + intToHexString( trailer->marker )
                            + ", trailer1 marker = " + intToHexString( msg1Trailer->marker ) );
            notEqual = true;
        }

        for( int b = 0; b < getLength(); b++ )
        {
            if( messageBuffer[b] != msg1.messageBuffer[b] )
            {
                logger->logError( "Strings not equal for byte number " + to_string( b ) );
                notEqual = true;
                break;
            }
        }
    }

    return notEqual;
}
