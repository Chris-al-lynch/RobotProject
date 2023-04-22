#ifndef _MESSAGE_PROCESSOR_H_
#define _MESSAGE_PROCESSOR_H_

#include <thread>

#include "Logger.h"
#include "MessageQueue.h"
#include "RawBuffer.h"
#include "ResponseQueue.h"

using namespace std;

class MessageProcessor
{
    private:
        Logger *logger;
        jthread *processorThread;
        MessageQueue *messageQueue;
        ResponseQueue *responseQueue;
        
        static void processor( MessageProcessor *this_p );
        RawBuffer *processMessage( RawBuffer *messageBuffer );

    public:
        MessageProcessor();
        ~MessageProcessor();
        void start();
};

#endif