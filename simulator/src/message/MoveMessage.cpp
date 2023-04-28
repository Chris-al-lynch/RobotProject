
#include "Exceptions.h"
#include "MoveMessage.h"

MoveMessage::MoveMessage() : Message()
{
}

MoveMessage::MoveMessage( MoveMessageId id, int amount )
             : Message( MessageHeader::getHeaderSize() + sizeof( int )
                      + MessageTrailer::getTrailerSize(),
                        MOVEMENT_MESSAGE, id, MOVE_MESSAGE_VERSION )
{
    this->amount = amount;
}

MoveMessage::~MoveMessage()
{
}

void
MoveMessage::setAmount( int amount )
{
    this->amount = amount;
}

MoveMessageResponse *
MoveMessage::processMessage( MessageProcessor *messageProcessor )
{
    MoveResponseId responseId = MOVE_RESPONSE_SUCCESS;
    string error;

    try
    {
        switch( header->getId() )
        {
            case MOVE_FORWARD:
            {
                moveFunction( messageProcessor, MOVE_FORWARD, amount );
                break;
            }
            case MOVE_BACKWARD:
            {
                moveFunction( messageProcessor, MOVE_BACKWARD, amount );
                break;
            }
            case MOVE_LEFT:
            {
                moveFunction( messageProcessor, MOVE_LEFT, amount );
                break;
            }
            case MOVE_RIGHT:
            {
                moveFunction( messageProcessor, MOVE_RIGHT, amount );
                break;
            }
            case STOP:
            {
                stopFunction( messageProcessor );
                break;
            }
            case INVALID_MOVE_MESSAGE:
            default:
            {
                break;
            }
        }
    }
    catch( exception& e )
    {
        error = string( e.what() );
        responseId = MOVE_RESPONSE_FAILURE;
    }

    return new MoveMessageResponse( responseId, error );
}

void
MoveMessage::pack( RawBuffer *buffer )
{
    header->pack( buffer );
    buffer->putInt( amount );
    trailer->pack( buffer );
}

MoveMessage *
MoveMessage::unpack( RawBuffer *buffer )
{
    MoveMessage *mm = new MoveMessage();
    mm->header = MessageHeader::unpack( buffer );
    mm->trailer = MessageTrailer::unpack( buffer );
 
    int dataLength = mm->header->getLength() - MessageHeader::getHeaderSize()
                   - MessageTrailer::getTrailerSize();

    if( dataLength != sizeof( int ) )
    {
        throw MessageException( "MoveMessage received an invalid header: Expected data size to be '"
                              + to_string( sizeof( int ) ) + "', received data size '"
                              + to_string( dataLength ) + "'" );
    }

    buffer->setPosition( MessageHeader::getHeaderSize() );
    mm->setAmount( buffer->getInt() );

    return mm;
}