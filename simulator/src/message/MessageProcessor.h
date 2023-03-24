#ifndef _MESSAGE_PROCESSOR_H_
#define _MESSAGE_PROCESSOR_H_

#include <thread>

#include "Logger.h"
#include "MessageQueue.h"

using namespace std;

class MessageProcessor
{
    private:
        Logger *logger;
        jthread *processorThread;
        MessageQueue *messageQueue;
        
        static void processor( MessageProcessor *this_p );
        char *processMessage( char *messageBuffer );

    public:
        MessageProcessor();
        ~MessageProcessor();
        void start();


};

#endif