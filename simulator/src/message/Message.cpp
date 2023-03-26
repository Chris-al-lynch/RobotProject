
#include <climits>
#include <cstdlib>

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
    
    if( header->marker != MSG_HEADER_MARKER )
    {
        return false;
    }

    msgTrailer *trailer = (msgTrailer *)&messageBuffer[header->msgLength - sizeof( msgTrailer )];

    if( trailer->marker != MSG_TRAILER_MARKER )
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
        type = (messageType_t)(((msgHeader *)messageBuffer)->msgType);
    }

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
        id = ((msgHeader *)messageBuffer)->id;
    }

    return id;
}

int
Message::getLength()
{
    int length = -1;

    if( messageBuffer != nullptr )
    {
        length = ((msgHeader *)messageBuffer)->msgLength;
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
