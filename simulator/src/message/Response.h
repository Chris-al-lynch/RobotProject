#ifndef __RESPONSE_H__
#define __RESPONSE_H__

#include "MessageHeader.h"
#include "MessageTrailer.h"
#include "RawBuffer.h"
#include "ResponseType.h"

const int RESPONSE_VERSION = 0;

class Response
{
    protected:
        MessageHeader  *header;
        MessageTrailer *trailer;

        Response();

        Response( int dataLength, int type, int id, int sequenceId, int version );
        Response( int dataLength, int type, int id, int version );

    public:
        ~Response();
        virtual void pack( RawBuffer *buffer ) = 0;

        MessageHeader  *getHeader();
        MessageTrailer *getTrailer();

};

#endif