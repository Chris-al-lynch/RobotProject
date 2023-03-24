#ifndef _TEST_MESSAGE_H_
#define _TEST_MESSAGE_H_

#include "Message.h"

using namespace std;

typedef struct testMessage
{
   char *message;
} testMessage;

typedef struct testResponse
{
   char *message;
} testResponse;

class TestMessage : public Message
{
    private:
        char *generateTestResponse( string message );
    public:
        TestMessage( char *messageBuffer );
        char *processMessage();
        char *generateTestMessage( string message );
};

#endif