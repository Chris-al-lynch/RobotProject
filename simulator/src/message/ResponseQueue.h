#ifndef _RESPONSE_QUEUE_H_
#define _RESPONSE_QUEUE_H_

#include <mutex>
#include <queue>
#include <semaphore>

#include "MessageQueueInterface.h"
#include "RawBuffer.h"

using namespace std;

class ResponseQueue : public MessageQueueInterface
{
    private:
       queue<messageTransfer_t> *responseQueue;
       mutex *queueLock;
       counting_semaphore<1> *semaphore;

       static ResponseQueue *instance;

       ResponseQueue();

    public:
       ~ResponseQueue();

       /* Singleton should not be cloneable */
       ResponseQueue( ResponseQueue &other ) = delete;
       /* Singleton should not be assignable */
       void operator=( const ResponseQueue & ) = delete;
       static ResponseQueue *getInstance();
       void addMessage( messageTransfer_t msg ) override;
       void retrieveMessage( messageTransfer_t *msg ) override;
};

#endif