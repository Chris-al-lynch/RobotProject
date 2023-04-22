
#include <cstdlib>
#include <cstring>

#include "Exceptions.h"
#include "RawBuffer.h"

RawBuffer::RawBuffer( size_t bufferSize )
{
    this->index = 0;
    this->bufferSize = bufferSize;
    this->buffer = (char *)calloc( bufferSize, sizeof( char ) );
    this->dataSize = 0;
    this->markValid = false;
}

RawBuffer::RawBuffer( char *buffer, size_t bufferSize )
{
    this->index = 0;
    this->bufferSize = bufferSize;
    this->buffer = (char *)calloc( bufferSize, sizeof( char ) );
    this->dataSize = 0;
    this->markValid = false;

    memcpy( this->buffer, buffer, bufferSize );
}

RawBuffer::~RawBuffer()
{
    free( this->buffer );
}

size_t
RawBuffer::getBufferSize()
{
    return bufferSize;
}

size_t
RawBuffer::getDataSize()
{
    return dataSize;
}

size_t
RawBuffer::getPosition()
{
    return index;
}

void
RawBuffer::setPosition( size_t position )
{
    index = position;
}

void
RawBuffer::rewind()
{
    index = 0;
}

void
RawBuffer::mark()
{
    markIndex = index;
    markValid = true;
}

void
RawBuffer::reset()
{
    if( markValid )
    {
        index = markIndex;
    }
    // FIXME: Should throw an exception if the mark is not valid.  Shouldn't 
    // be calling this method if mark() was never called.
}

void
RawBuffer::clear()
{
    markIndex = 0;
    index = 0;
    markValid = false;
}

char *
RawBuffer::getBuffer()
{
    return buffer;
}

RawBuffer *
RawBuffer::getBuffer( size_t offset, size_t length )
{
    return new RawBuffer( &buffer[offset], length );
}

void
RawBuffer::putInt( int data )
{
    if( (index + sizeof( int )) > bufferSize )
    {
        throw BufferOverflowException( "Adding this int will overflow the buffer." );
    }

    memcpy( &buffer[index], &data, sizeof( int ) );
    index += sizeof( int );
    if( index > dataSize )
    {
        dataSize = index;
    }
}

void
RawBuffer::putInt( int data, size_t index )
{
    if( (index + sizeof( int )) > bufferSize )
    {
        throw BufferOverflowException( "Adding this int will overflow the buffer." );
    }

    memcpy( &buffer[index], &data, sizeof( int ) );
    if( (index + sizeof( int )) > dataSize )
    {
        dataSize = index + sizeof( int );
    }
}

void
RawBuffer::putShort( short data )
{
    if( (index + sizeof( short )) > bufferSize )
    {
        throw BufferOverflowException( "Adding this short will overflow the buffer." );
    }

    memcpy( &buffer[index], &data, sizeof( short ) );
    index += sizeof( short );

    if( index > dataSize )
    {
        dataSize = index;
    }
}

void
RawBuffer::putShort( short data, size_t index )
{
    if( (index + sizeof( short )) > bufferSize )
    {
        throw BufferOverflowException( "Adding this short will overflow the buffer." );
    }

    memcpy( &buffer[index], &data, sizeof( short ) );

    if( (index + sizeof( short )) > dataSize )
    {
        dataSize = index + sizeof( short );
    }
}

void
RawBuffer::putChar( char data )
{
    if( (index + sizeof( char )) > bufferSize )
    {
        throw BufferOverflowException( "Adding this char will overflow the buffer." );
    }

    buffer[index] = data;
    index += sizeof( char );

    if( index > dataSize )
    {
        dataSize = index;
    }
}

void
RawBuffer::putChar( char data, size_t index )
{
    if( (index + sizeof( char )) > bufferSize )
    {
        throw BufferOverflowException( "Adding this char will overflow the buffer." );
    }

    buffer[index] = data;

    if( (index + sizeof( char )) > dataSize )
    {
        dataSize = index + sizeof( char );
    }
}

void
RawBuffer::putLong( long data )
{
    if( (index + sizeof( long )) > bufferSize )
    {
        throw BufferOverflowException( "Adding this long will overflow the buffer." );
    }

    memcpy( &buffer[index], &data, sizeof( long ) );
    index += sizeof( long );

    if( index > dataSize )
    {
        dataSize = index;
    }
}

void
RawBuffer::putLong( long data, size_t index )
{
    if( (index + sizeof( long )) > bufferSize )
    {
        throw BufferOverflowException( "Adding this long will overflow the buffer." );
    }

    memcpy( &buffer[index], &data, sizeof( short ) );

    if( (index + sizeof( long )) > dataSize )
    {
        dataSize = index + sizeof( long );
    }
}

void
RawBuffer::put( char *data, size_t size )
{
    if( (index + size) > bufferSize )
    {
        throw BufferOverflowException( "Adding this data will overflow the buffer." );
    }

    memcpy( &buffer[index], data, size );
    index += size;
}

void
RawBuffer::put( char *data, size_t bufferOffset, size_t size )
{
    if( (bufferOffset + size) > bufferSize )
    {
        throw BufferOverflowException( "Adding this data at this offset will overflow the buffer." );
    }

    memcpy( &buffer[bufferOffset], data, size );

    if( (bufferOffset + size) > dataSize )
    {
        dataSize = bufferOffset + size;
    }
}

void
RawBuffer::put( char *data, size_t dataOffset,
                size_t bufferOffset, size_t size )
{
    if( (bufferOffset + size) > bufferSize )
    {
        throw BufferOverflowException( "Adding this data at this offset will overflow the buffer." );
    }

    memcpy( &buffer[bufferOffset], &data[dataOffset], size );

    if( (bufferOffset + size) > dataSize )
    {
        dataSize = bufferOffset + size;
    }
}

void
RawBuffer::put( RawBuffer buffer )
{
    if( (index + buffer.getDataSize()) > bufferSize )
    {
        throw BufferOverflowException( "Adding this raw buffer will overflow the buffer." );
    }

    buffer.get( &(this->buffer[index]), bufferSize - index );
    index += buffer.getDataSize();
}

void RawBuffer::put( RawBuffer buffer, size_t size )
{
    if( (index + size) > bufferSize )
    {
        throw BufferOverflowException( "Adding this much data will overflow the buffer." );
    }

    buffer.get( &(this->buffer[index]), bufferSize - index );
    index += size;

    if( index > dataSize )
    {
        dataSize = index;
    }
}

void
RawBuffer::put( RawBuffer buffer, size_t destOffset, size_t size )
{
    if( (destOffset + size) > bufferSize )
    {
        throw BufferOverflowException( "Adding this much data will overflow the buffer." );
    }

    buffer.get( &(this->buffer[destOffset]), bufferSize - destOffset );
}

int
RawBuffer::getInt()
{
    if( (index + sizeof( int ) ) > bufferSize )
    {
        throw BufferOverflowException( "Retrieving an int from this location will go past end of buffer." );
    }

    int num = *((int*)&buffer[index]);
    index += sizeof( int );
    return num;
}

int
RawBuffer::getInt( size_t index )
{
    if( (index + sizeof( int ) ) > bufferSize )
    {
        throw BufferOverflowException( "Retrieving an int from this location will go past end of buffer." );
    }

    return *((int*)&buffer[index]);
}

short
RawBuffer::getShort()
{
    if( (index + sizeof( short ) ) > bufferSize )
    {
        throw BufferOverflowException( "Retrieving an short from this location will go past end of buffer." );
    }

    short num = *((short*)&buffer[index]);
    index += sizeof( short );
    return num;
}

short
RawBuffer::getShort( size_t index )
{
    if( (index + sizeof( short ) ) > bufferSize )
    {
        throw BufferOverflowException( "Retrieving an short from this location will go past end of buffer." );
    }

    return *((short*)&buffer[index]);
}

char
RawBuffer::getChar()
{
    if( (index + 1) > bufferSize )
    {
        throw BufferOverflowException( "Retrieving a byte from this location will go past end of buffer." );
    }

    char c = buffer[index];
    index++;
    return c;
}

char
RawBuffer::getChar( size_t index )
{
    if( (index + 1) > bufferSize )
    {
        throw BufferOverflowException( "Retrieving a byte from this location will go past end of buffer." );
    }

    return buffer[index];
}

long
RawBuffer::getLong()
{
    if( (index + sizeof( long )) > bufferSize )
    {
        throw BufferOverflowException( "Retrieving a long from this location will go past end of buffer." );
    }

    long num = *((long*)&buffer[index]);
    index += sizeof( long );
    return num;
}

long
RawBuffer::getLong( size_t index )
{
    if( (index + sizeof( long )) > bufferSize )
    {
        throw BufferOverflowException( "Retrieving a long from this location will go past end of buffer." );
    }

    return *((long*)&buffer[index]);
}

void
RawBuffer::get( char *destination, size_t destinationSize )
{
    if( destinationSize < dataSize )
    {
        throw BufferOverflowException( "Buffer size too small for data." );
    }

    memcpy( destination, this->buffer, dataSize );
}

void
RawBuffer::get( char *destination, size_t offset, size_t size )
{
    if( size > (dataSize - index) )
    {
        throw BufferOverflowException( "Buffer size too small for data." );
    }

    memcpy( &destination[offset], &buffer[index], size );
}

string
RawBuffer::toString()
{
    char array[1024];

    for( int a = 0, b = 0; b < bufferSize; a+=3, b++ )
    {
        if( (b != 0) && ((b%8) == 0) )
        {
            sprintf( &array[a], "\n" );
            a++;
        }
        sprintf( &array[a], "%02X ", buffer[b] );
    }

    string str( array );

    return str;
}