
#include <climits>
#include <cstdlib>
#include <iostream>

#include "Exceptions.h"
#include "Message.h"

int Message::messageIdentifier = 0;

Message::Message( char *messageBuffer )
{
    if( messageBuffer == nullptr )
    {
        throw MessageException( "Invalid NULL messageBuffer passed to Message constructor." );
    }

    this->messageBuffer = messageBuffer;
    logger = new Logger();
}

bool
Message::isMessageValid()
{
    msgHeader *header = (msgHeader *)messageBuffer;

    int marker = __builtin_bswap32( header->marker );

    printf( "header marker = 0x%08X\n", marker );
    
    if( marker != MSG_HEADER_MARKER )
    {
        return false;
    }

    msgTrailer *trailer = (msgTrailer *)&messageBuffer[header->msgLength - sizeof( msgTrailer )];

    marker = __builtin_bswap32( trailer->marker );
    printf( "trailer marker = 0x%08X\n", marker );

    if( marker != MSG_TRAILER_MARKER )
    {
        return false;
    }

    return true;
}

messageType_t
Message::getType()
{
    messageType_t type = INVALID_MESSAGE;

    if( messageBuffer != nullptr )
    {
        type = (messageType_t)__builtin_bswap32( ((msgHeader *)messageBuffer)->msgType );
    }

    cout << "type = " + to_string(type ) << endl;

    return type;
}

int
Message::getMessageIdForNewMessage()
{
    return ++(Message::messageIdentifier);
}

int
Message::getId()
{
    int id = -1;
    
    if( messageBuffer != nullptr )
    {
        id = __builtin_bswap32( ((msgHeader *)messageBuffer)->id );
    }

    return id;
}

int
Message::getLength()
{
    int length = -1;

    if( messageBuffer != nullptr )
    {
        length = __builtin_bswap32( ((msgHeader *)messageBuffer)->msgLength );
    }

    return length;
}

void
Message::generateMessageHeader( msgHeader *hdr, int length, messageType_t type )
{
    hdr->msgLength = length;
    hdr->msgType   = type;
    hdr->id        = getMessageIdForNewMessage();
    hdr->marker    = MSG_HEADER_MARKER;
}

void
Message::generateResponseHeader( msgHeader *hdr, int length,
                                 messageType_t type, int id )
{
    hdr->msgLength = length;
    hdr->msgType   = type;
    hdr->id        = id;
    hdr->marker    = MSG_HEADER_MARKER;
}

void
Message::generateTrailer( msgTrailer *trailer )
{
    trailer->marker = MSG_TRAILER_MARKER;
}
