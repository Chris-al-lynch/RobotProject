#ifndef __MESSAGE_HEADER_H__
#define __MESSAGE_HEADER_H__

#include "MessageType.h"
#include "RawBuffer.h"

class MessageHeader
{
    private:
        static int CurrentSequenceId;
        int length;
        int type;
        int id;
        int sequenceId;
        int version;
        int marker;
        static const int HEADER_MARKER = 0xBAD00DAD;



    public:
        MessageHeader( int length, int type, int id, int version );
        MessageHeader( int length, int type, int id, int sequenceId, int version );
        ~MessageHeader();

        static MessageHeader *unpack( RawBuffer *buffer );
        void pack( RawBuffer *buffer );
        static int getHeaderSize();

        int getSize();
        int getLength();
        int getType();
        int getId();
        int getVersion();

};

#endif
