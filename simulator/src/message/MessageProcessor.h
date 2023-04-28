#ifndef _MESSAGE_PROCESSOR_H_
#define _MESSAGE_PROCESSOR_H_

#include <thread>

#include "Logger.h"
#include "MessageQueue.h"
#include "RawBuffer.h"
#include "ResponseQueue.h"
#include "Status.h"

using namespace std;

class MessageProcessor
{
    private:
        Logger *logger;
        jthread *processorThread;
        MessageQueue *messageQueue;
        ResponseQueue *responseQueue;
        Status status;
        friend class Message;
        
        static void processor( MessageProcessor *this_p );
        RawBuffer *processMessage( RawBuffer *messageBuffer );

    protected:
        void (*moveFunction)( int direction, unsigned int amount );
        void (*stopFunction)();

    public:
        MessageProcessor();
        ~MessageProcessor();

        void registerMoveFunction( void (*moveFunction)( int direction,
                                                         unsigned int amount ) );
        void registerStopFunction( void (*stopFunction)() );

        Status getStatus();
};

#endif