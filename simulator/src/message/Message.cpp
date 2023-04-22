
#include "Message.h"

Message::Message()
{
}

Message::Message( int dataLength, int type, int id, int sequenceId, int version )
{
    header = new MessageHeader( MessageHeader::getHeaderSize() + dataLength + MessageTrailer::getTrailerSize(),
                                type, id, sequenceId, version );
    trailer = new MessageTrailer();
}

Message::Message( int dataLength, int type, int id, int version )
{
    header = new MessageHeader( MessageHeader::getHeaderSize() + dataLength + MessageTrailer::getTrailerSize(),
                                type, id, version );
    trailer = new MessageTrailer();
}

Message::~Message()
{
    if( trailer != nullptr )
    {
        delete trailer;
    }

    if( header != nullptr )
    {
        delete header;
    }
}

MessageHeader *
Message::getHeader()
{
    return header;
}

MessageTrailer *
Message::getTrailer()
{
    return trailer;
}
