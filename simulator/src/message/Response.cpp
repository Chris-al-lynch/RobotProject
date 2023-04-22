
#include "Response.h"

Response::Response()
{
    header  = nullptr;
    trailer = nullptr;
}

Response::Response( int dataLength, int type, int id, int sequenceId, int version )
{
    header = new MessageHeader( MessageHeader::getHeaderSize() + dataLength
                              + MessageTrailer::getTrailerSize(), type, id,
                                sequenceId, version );
    trailer = new MessageTrailer();
}

Response::Response( int dataLength, int type, int id, int version )
{
    header = new MessageHeader( MessageHeader::getHeaderSize() + dataLength
                              + MessageTrailer::getTrailerSize(), type, id,
                                version );
    trailer = new MessageTrailer();
}

Response::~Response()
{
    if( header != nullptr )
    {
        delete header;
    }

    if( trailer != nullptr )
    {
        delete trailer;
    }
}

MessageHeader *
Response::getHeader()
{
    return header;
}

MessageTrailer *
Response::getTrailer()
{
    return trailer;
}
