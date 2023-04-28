#ifndef __MOVE_MESSAGE_H__
#define __MOVE_MESSAGE_H__

#include <limits.h>

#include "Message.h"
#include "MessageProcessor.h"
#include "MoveMessageId.h"
#include "MoveMessageResponse.h"

using namespace std;

class MoveMessage : public Message
{
    private:
        static const int MOVE_MESSAGE_VERSION = 0;
        static const int MOVE_FOREVER = INT_MAX;
        int amount;
        MoveMessage();
        void setAmount( int amount );

    public:
        MoveMessage( MoveMessageId id, int amount );
        ~MoveMessage();

        MoveMessageResponse *processMessage( MessageProcessor *messageProcessor ) override;
        void pack( RawBuffer *buffer ) override;
        static MoveMessage *unpack( RawBuffer *buffer );

};

#endif