#ifndef __MESAGE_TRAILER_H__
#define __MESAGE_TRAILER_H__

#include "RawBuffer.h"

class MessageTrailer
{
    private:
        int marker;
        static const int TRAILER_MARKER = 0xDAD00BAD;


    public: 
        MessageTrailer();
        ~MessageTrailer();

        //static MessageTrailer *generateTrailer();
        static int getTrailerSize();

        static MessageTrailer *unpack( RawBuffer *buffer );
        void pack( RawBuffer *buffer );

        int getSize();
};

#endif
