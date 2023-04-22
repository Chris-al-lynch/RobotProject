
#include "Message.h"
#include "MessageType.h"
#include "TestMessage.h"
#include "TestMessageId.h"
#include "TestResponseId.h"

TestMessage::TestMessage() : Message()
{

}

TestMessage::TestMessage( string message )
           : Message( MessageHeader::getHeaderSize() + message.length() + MessageTrailer::getTrailerSize(),
                      TEST_MESSAGE, TEST_MESSAGE_STRING, TEST_MESSAGE_VERSION )
{
    this->message = message;
}

TestMessage::~TestMessage()
{
}

string 
TestMessage::getMessage()
{
    return message;
}

TestMessageResponse *
TestMessage::processMessage()
{
    return new TestMessageResponse( TEST_RESPONSE_SUCCESS,
                                    "Received message: " + message );
}

void
TestMessage::pack( RawBuffer *buffer )
{
    buffer->put( (char *)message.c_str(), message.length() );
}

TestMessage *
TestMessage::unpack( RawBuffer *buffer )
{
    TestMessage *tm = new TestMessage();
    tm->header  = MessageHeader::unpack( buffer );
    tm->trailer = MessageTrailer::unpack( buffer );

    int dataLength = tm->header->getLength() - MessageHeader::getHeaderSize()
                   - MessageTrailer::getTrailerSize();
    char *data = new char[dataLength+1];
    buffer->setPosition( MessageHeader::getHeaderSize() );
    buffer->get( data, 0, dataLength );
    data[dataLength] = '\0';

    tm->message.append( data );

    delete data;

    return tm;
}

