

#include "Exceptions.h"
#include "MessageFactory.h"
#include "MessageHeader.h"
#include "MessageTrailer.h"
#include "TestMessage.h"

Message *
MessageFactory::unpack( RawBuffer *buffer )
{
    MessageHeader *header  = MessageHeader::unpack( buffer );
    Message       *message = nullptr;

    switch( header->getType() )
    {
        case MOVEMENT_MESSAGE:
        {
            //MovementMessageFactory
            break;
        }
        case SENSE_MESSAGE:
        {
            //SenseMessageFactory
            break;
        }
        case TEST_MESSAGE:
        {
            message = TestMessage::unpack( buffer );
            break;
        }
        case INVALID_MESSAGE:
        default:
        {
            break;
        }
    }

    return message;
}