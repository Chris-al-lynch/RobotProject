#ifndef _TEST_MESSAGE_H_
#define _TEST_MESSAGE_H_

#include "Message.h"
#include "RawBuffer.h"
#include "TestMessageResponse.h"

using namespace std;

class TestMessage : public Message
{
    private:
        static const int TEST_MESSAGE_VERSION = 0;
        string message;
        TestMessage();
        
    public:
        TestMessage( string message );
        ~TestMessage();

        string getMessage();

        TestMessageResponse *processMessage() override;
        void pack( RawBuffer *buffer ) override;
        static TestMessage *unpack( RawBuffer *buffer );
};

#endif