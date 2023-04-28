
#include "Exceptions.h"
#include "MoveMessageResponse.h"

MoveMessageResponse::MoveMessageResponse() : Response()
{
    header       = nullptr;
    trailer      = nullptr;
}

MoveMessageResponse::MoveMessageResponse( MoveResponseId id, string errorMessage )
{
    header = new MessageHeader( MessageHeader::getHeaderSize() + sizeof( int ) + MessageTrailer::getTrailerSize(),
                                MOVEMENT_MESSAGE, id, MOVE_RESPONSE_VERSION );
    trailer = new MessageTrailer();
    this->errorMessage = errorMessage;
}

void
MoveMessageResponse::setErrorMessage( string errorMessage )
{
    this->errorMessage = errorMessage;
}

void
MoveMessageResponse::pack( RawBuffer *buffer )
{
    header->pack( buffer );
    if( not errorMessage.empty() )
    {
        buffer->put( (char *)errorMessage.c_str(), errorMessage.length() );
    }
    trailer->pack( buffer );
}

MoveMessageResponse *
MoveMessageResponse::unpack( RawBuffer *buffer )
{
    MoveMessageResponse *mmr = new MoveMessageResponse();

    mmr->header = MessageHeader::unpack( buffer );
    mmr->trailer = MessageTrailer::unpack( buffer );
    int dataLength = mmr->header->getLength() - MessageHeader::getHeaderSize()
                   - MessageTrailer::getTrailerSize();
    if( dataLength != 0 )
    {
        char *data = new char[dataLength+1];
        buffer->get( data, MessageHeader::getHeaderSize(), dataLength );
        data[dataLength] = '\0';
        mmr->setErrorMessage( string( data ) );
    }

    return mmr;
}