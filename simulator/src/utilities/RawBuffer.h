#ifndef __RAW_MESSAGE_BUFFER_H__
#define __RAW_MESSAGE_BUFFER_H__

#include <sys/types.h>
#include <string>

using namespace std;

/**
 * Class for reading and writing to a buffer.  This class is similar to
 * ByteBuffer in Java.
 */
class RawBuffer
{
    private:
        /* Pointer to the buffer. */
        char  *buffer;
        /* Size of the buffer. */
        size_t bufferSize;
        /* Current position */
        size_t index;
        /* Amount of data stored in the buffer. */
        size_t dataSize;
        /* Stored location that can be recalled. */
        size_t markIndex;
        /* Is the current mark valid? */
        bool markValid;

    public:
        /**
         * Constructor that allocates a buffer.
         * @param bufferSize Size of the buffer to be allocated.
         */
        RawBuffer( size_t bufferSize );

        /**
         * Constructor that allocates a buffer and copies data into it.
         * @param buffer Buffer to be copied
         * @param bufferSize Size of the buffer to be copied.
         */
        RawBuffer( char* buffer, size_t bufferSize );

        /**
         * Destructor that will free the allocated buffer.
         */
        ~RawBuffer();

        /**
         * Retrieve the buffer size.
         * 
         * @return Size of the internal buffer.
         */
        size_t getBufferSize();

        /**
         * Retrieve the data size.
         * 
         * @return Size of the data stored in the internal buffer.
         */
        size_t getDataSize();

        /**
         * Get the current position 
         * 
         * @return current position
         */
        size_t getPosition();

        /**
         * Set the position to a new location.
         * 
         * @param position New position
         */
        void setPosition( size_t position );

        /**
         * Set the current position to 0.
         */
        void rewind();

        /**
         * Save away the current position to be recalled later with reset.
         */
        void mark();
        /**
         * Set the current position back to mark.
         */
        void reset();
        /**
         * Reset all positions back to 0 and invalidate the mark.
         */
        void clear();

        /**
         * Return a copy of the buffer at the specified offset and length.
         * 
         * @param offset Offset in the buffer to start the copy.
         * @param length amount of data to copy from the buffer.
         * @return a RawBuffer with the copied buffer.
         */
        RawBuffer *getBuffer( size_t offset, size_t length );

        /**
         * Retrieve a pointer to the internal buffer.  Changing the returned
         * buffer will change the RawBuffer and changing the RawBuffer will
         * change the returned buffer.
         * 
         * @return pointer to the internal buffer.
         */
        char *getBuffer();

        /**
         * Store an integer in the buffer at the current position.
         * Position will be updated to point immediately past the new integer.
         * 
         * @param data Integer to store.
         */
        void putInt( int data );

        /**
         * Store an integer in the buffer at the specified position.
         * Position will not be updated.
         * 
         * @param data Integer to store.
         * @param position Position to store the integer.
         */
        void putInt( int data, size_t position );

        /**
         * Store a short in the buffer at the current position.
         * Position will be updated to point immediately past the new short.
         * 
         * @param data Short to store.
         */
        void putShort( short data );

        /**
         * Store a short in the buffer at the specified position.
         * Position will not be updated.
         * 
         * @param data Short to store.
         * @param position Position to store the short.
         */
        void putShort( short data, size_t position );

        /**
         * Store a char in the buffer at the current position.
         * Position will be updated to point immediately past the new char.
         * 
         * @param data Char to store.
         */
        void putChar( char data );

        /**
         * Store a char in the buffer at the specified position.
         * Position will not be updated.
         * 
         * @param data Char to store.
         * @param position Position to store the char.
         */
        void putChar( char data, size_t position );

        /**
         * Store a long in the buffer at the current position.
         * Position will be updated to point immediately past the new long.
         * 
         * @param data Long to store.
         */
        void putLong( long data );

        /**
         * Store a long in the buffer at the specified position.
         * Position will not be updated.
         * 
         * @param data Long to store.
         * @param position Position to store the long.
         */
        void putLong( long data, size_t position );

        /**
         * Store 'size' bytes of data pointed to by 'data' starting at the 
         * current position.  Position will be updated to just past the 
         * new data.
         * 
         * @param data Data to store.
         * @param size Amount of data to store.
         */
        void put( char *data, size_t size );

        /**
         * Store 'size' bytes of data pointed to by 'data' into the RawBuffer
         * starting at the specified 'bufferOffset'.  Position will not be updated.
         * 
         * @param data Data to store.
         * @param bufferOffset Offset into the RawBuffer.
         * @param size Amount of data to store.
         */
        void put( char *data, size_t bufferOffset, size_t size );

        /**
         * Store 'size' bytes of data pointed to by 'data' at the specified
         * 'dataOffset' into the RawBuffer starting at the specified
         * 'bufferOffset'.  Position will not be updated.
         * 
         * @param data Data to store.
         * @param dataOffset Offset into data
         * @param bufferOffset Offset into the RawBuffer.
         * @param size Amount of data to store.
         */
        void put( char *data, size_t dataOffset, size_t bufferOffset, size_t size );

        /**
         * Store the entire contents of the specified buffer into the buffer
         * starting at the current position. The position will be updated to
         * just past the new data.
         * 
         * If the specified buffer is too large to fit in the remaining space between
         * the current position and the end of the buffer, a BufferOverflowException
         * will be thrown.
         */
        void put( RawBuffer buffer );

        /**
         * Store 'size' bytes of the specified buffer into the buffer
         * starting at the current position. The position will be updated to
         * just past the new data.
         * 
         * If size is too large to fit in the remaining space between
         * the current position and the end of the buffer, a BufferOverflowException
         * will be thrown.
         */
        void put( RawBuffer buffer, size_t size );

        /**
         * Store 'size' bytes of the specified buffer into the buffer
         * starting at 'destOffset'. The position will not be updated.
         * 
         * If size is too large to fit in the space between 'destOffset' and
         * the end of the buffer, a BufferOverflowException will be thrown.
         */
        void put( RawBuffer buffer, size_t destOffset, size_t size );

        /**
         * Retrieve an integer starting from the current position in the buffer.
         * Position will be updated to just past the integer retrieved.
         * 
         * @return integer retrieved.
         */
        int getInt();

        /**
         * Retrieve an integer starting from the specified 'position' in the
         * buffer.  Position will not be updated.
         * 
         * @param position Position to retrieve the integer.
         * @return integer retrieved.
         */
        int getInt( size_t position );

        /**
         * Retrieve a short starting from the current position in the buffer.
         * Position will be updated to just past the short retrieved.
         * 
         * @return short retrieved.
         */
        short getShort();

        /**
         * Retrieve a short starting from the specified 'position' in the
         * buffer.  Position will not be updated.
         * 
         * @param position Position to retrieve the short.
         * @return short retrieved.
         */
        short getShort( size_t position );

        /**
         * Retrieve a char starting from the current position in the buffer.
         * Position will be updated to just past the char retrieved.
         * 
         * @return char retrieved.
         */
        char getChar();

        /**
         * Retrieve a char starting from the specified 'position' in the
         * buffer.  Position will not be updated.
         * 
         * @param position Position to retrieve the char.
         * @return char retrieved.
         */
        char getChar( size_t position );

        /**
         * Retrieve a long starting from the current position in the buffer.
         * Position will be updated to just past the long retrieved.
         * 
         * @return long retrieved.
         */
        long getLong();

        /**
         * Retrieve a long starting from the specified 'position' in the
         * buffer.  Position will not be updated.
         * 
         * @param position Position to retrieve the long.
         * @return long retrieved.
         */
        long getLong( size_t position );

        /**
         * Store the data from the RawBuffer into the specified 'destination'
         * with the specified size. If 'destinationSize' is smaller than the 
         * data in the RawBuffer, then a BufferOverflowException will be
         * thrown.
         * Position will not be updated.
         * 
         * @param destination Location to store buffer data.
         * @param destinationSize Size of the destination buffer.
         */
        void get( char *destination, size_t destinationSize );

        /**
         * Store the data from the RawBuffer into the specified 'destination'
         * at the specified 'offset' with the specified size. If 'destinationSize' is smaller than the 
         * data in the RawBuffer, then a BufferOverflowException will be
         * thrown.
         * Position will not be updated.
         * 
         * FIXME: This method is poorly implemented and should be removed.
         * 
         * @param destination Location to store buffer data.
         * @param size Size of the destination buffer.
         */
        void get( char *destination, size_t offset, size_t size );

        /**
         * This method is used for debugging.  It will generate a string
         * containg the buffer in rows of 8 bytes in hex.
         */
        string toString();
};

#endif
