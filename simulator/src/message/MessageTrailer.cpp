
#include "Exceptions.h"
#include "MessageTrailer.h"

MessageTrailer::MessageTrailer()
{
    marker = TRAILER_MARKER;
}

MessageTrailer::~MessageTrailer()
{
}

MessageTrailer *
MessageTrailer::unpack( RawBuffer *buffer )
{
    /* The trailer is the last part of the message.  The buffer contains the
     * entire message.  The length of the entire message is the first integer
     * in the message.  The trailer is located at length - size of trailer which
     * happens to be the size of the integer.
     */
    int trailerIndex = buffer->getInt( 0 ) - sizeof( int );
    int trailerMarker = buffer->getInt( trailerIndex );

    if( trailerMarker != TRAILER_MARKER )
    {
        throw InvalidParameterException( "Buffer does not have a valid trailer." );
    }

    return new MessageTrailer();
}

void
MessageTrailer::pack( RawBuffer *buffer )
{
    buffer->putInt( marker );
}

int
MessageTrailer::getTrailerSize()
{
    return sizeof( int );
}

int
MessageTrailer::getSize()
{
    return MessageTrailer::getTrailerSize();
}