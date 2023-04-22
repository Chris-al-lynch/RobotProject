#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "MessageHeader.h"
#include "MessageTrailer.h"
#include "RawBuffer.h"
#include "Response.h"

using namespace std;

class Message
{
    protected:
        MessageHeader *header;
        MessageTrailer *trailer;

        Message();
        Message( int dataLength, int type, int id, int sequenceId, int version );
        Message( int dataLength, int type, int id, int version );

    public:
       ~Message();
        MessageHeader *getHeader();
        MessageTrailer *getTrailer();

        virtual Response *processMessage() = 0;
        virtual void pack( RawBuffer *buffer ) = 0;
       
};

#endif