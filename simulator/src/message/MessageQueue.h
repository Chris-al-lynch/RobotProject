#ifndef _MESSAGE_QUEUE_H_
#define _MESSAGE_QUEUE_H_

#include <mutex>
#include <queue>
#include <semaphore>

#include "MessageQueueInterface.h"
#include "RawBuffer.h"

using namespace std;

class MessageQueue : public MessageQueueInterface
{
    private:
       queue<messageTransfer_t> *messageQueue;
       mutex *queueLock;
       counting_semaphore<1> *semaphore;

       static once_flag instanceFlag;
       static MessageQueue *instance;

       MessageQueue();

    public:
       ~MessageQueue();
       /* Singleton should not be cloneable */
       MessageQueue( MessageQueue &other ) = delete;
       /* Singleton should not be assignable */
       void operator=( const MessageQueue & ) = delete;

       static MessageQueue *getInstance();

       void addMessage( messageTransfer_t msg ) override;
       void retrieveMessage( messageTransfer_t *msg ) override;
};

#endif