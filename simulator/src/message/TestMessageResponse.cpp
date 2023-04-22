
#include "TestMessageResponse.h"

TestMessageResponse::TestMessageResponse()
{
    header  = nullptr;
    trailer = nullptr;
}

TestMessageResponse::TestMessageResponse( TestResponseId id, string message )
{
    header = new MessageHeader( MessageHeader::getHeaderSize() + message.length() + MessageTrailer::getTrailerSize(),
                                TEST_MESSAGE, id, TEST_RESPONSE_VERSION );
    trailer = new MessageTrailer();
    this->responseMessage = message;
}

string 
TestMessageResponse::getMessage()
{
    return responseMessage;
}

void
TestMessageResponse::setMessage( string message )
{
    responseMessage = message;
}

void
TestMessageResponse::pack( RawBuffer *buffer )
{
    header->pack( buffer );
    buffer->put( (char *)responseMessage.c_str(), responseMessage.length() );
    trailer->pack( buffer );
}

TestMessageResponse *
TestMessageResponse::unpack( RawBuffer *buffer )
{
    TestMessageResponse *tmr = new TestMessageResponse();

    tmr->header = MessageHeader::unpack( buffer );
    tmr->trailer = MessageTrailer::unpack( buffer );
    int dataLength = tmr->header->getLength();
    char *data = new char[dataLength+1];

    buffer->get( data, MessageHeader::getHeaderSize(), dataLength );
    data[dataLength] = '\0';
    tmr->setMessage( data );

    return tmr;
}
