#ifndef _MOVE_MESSAGE_RESPONSE_H_
#define _MOVE_MESSAGE_RESPONSE_H_

#include <string>

#include "RawBuffer.h"
#include "Response.h"
#include "MoveResponseId.h"

using namespace std;

class MoveMessageResponse : public Response
{
    private:
        static const int MOVE_RESPONSE_VERSION = 0;

        string errorMessage;

        MoveMessageResponse();

        void setErrorMessage( string errorMessage );

    public:
        MoveMessageResponse( MoveResponseId id, string errorMessage );

        void pack( RawBuffer *buffer ) override;

        static MoveMessageResponse *unpack( RawBuffer *buffer );
};

#endif