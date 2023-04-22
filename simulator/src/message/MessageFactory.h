#ifndef __MESSAGEFACTORY_H__
#define __MESSAGEFACTORY_H__

#include "Message.h"
#include "RawBuffer.h"

using namespace std;

class MessageFactory
{
    private:
    public:
        //static Message *createMessage( RawBuffer *buffer );
        static Message *unpack( RawBuffer *buffer );
};
#endif