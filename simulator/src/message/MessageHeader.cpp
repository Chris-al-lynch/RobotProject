
#include "Exceptions.h"
#include "MessageHeader.h"
#include "utils.h"

int MessageHeader::CurrentSequenceId = 0;

MessageHeader::MessageHeader( int length, int type, int id, int sequenceId, int version )
{
    this->length     = length;
    this->type       = type;
    this->id         = id;
    this->sequenceId = sequenceId;
    this->version    = version;
    this->marker     = HEADER_MARKER;
}

MessageHeader::MessageHeader( int length, int type, int id, int version )
             : MessageHeader( length, type, id, ++CurrentSequenceId, version )
{
}

MessageHeader::~MessageHeader()
{
}

MessageHeader *
MessageHeader::unpack( RawBuffer *buffer )
{
    buffer->mark();
    buffer->rewind();

    int length = buffer->getInt();
    int type = buffer->getInt();
    int id = buffer->getInt();
    int sequenceId = buffer->getInt();
    int version = buffer->getInt();
    int marker = buffer->getInt();

    buffer->reset();

    if( marker != HEADER_MARKER )
    {
        throw InvalidParameterException( "Buffer does not contain a valid header" );
    }

    return new MessageHeader( length, type, id, version );
}

void
MessageHeader::pack( RawBuffer *buffer )
{
    buffer->putInt( length );
    buffer->putInt( (int)type );
    buffer->putInt( id );
    buffer->putInt( sequenceId );
    buffer->putInt( version );
    buffer->putInt( marker );
}

int
MessageHeader::getHeaderSize()
{
    return sizeof( int ) + sizeof( int ) + sizeof( int ) + sizeof( int )
         + sizeof( int ) + sizeof( int );
}

int
MessageHeader::getSize()
{
    return MessageHeader::getHeaderSize();
}

int
MessageHeader::getLength()
{
    return length;
}

int
MessageHeader::getType()
{
    return type;
}

int
MessageHeader::getId()
{
    return id;
}

int
MessageHeader::getVersion()
{
    return version;
}
