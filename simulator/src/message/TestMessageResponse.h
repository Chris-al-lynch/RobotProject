#ifndef _TEST_MESSAGE_RESPONSE_H_
#define _TEST_MESSAGE_RESPONSE_H_

#include <string>

#include "RawBuffer.h"
#include "Response.h"
#include "TestResponseId.h"

using namespace std;

class TestMessageResponse : public Response
{
    private:
        static const int TEST_RESPONSE_VERSION = 0;
        string responseMessage;

        TestMessageResponse();

        void setMessage( string message );
        
    public:
        TestMessageResponse( TestResponseId id, string message );

        string getMessage();

        void pack( RawBuffer *buffer ) override;

        static TestMessageResponse *unpack( RawBuffer *buffer );
};

#endif