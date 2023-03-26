#ifndef _TEST_MESSAGE_H_
#define _TEST_MESSAGE_H_

#include "Message.h"

using namespace std;

class TestMessage : public Message
{
    private:
    public:
        TestMessage( char *messageBuffer );
        char *processMessage();
        char *generateTestMessage( string message );
        static char *generateTestResponse( string message, int id );
        bool operator!=( TestMessage &msg1 );
};

#endif