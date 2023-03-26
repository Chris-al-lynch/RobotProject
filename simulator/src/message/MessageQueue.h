#ifndef _MESSAGE_QUEUE_H_
#define _MESSAGE_QUEUE_H_

#include <mutex>
#include <queue>
#include <semaphore>

using namespace std;

typedef struct messageTransfer
{
    int connection;
    char *msgBuffer;
    char *respBuffer;
} messageTransfer_t;

class MessageQueue
{
    private:
       queue<messageTransfer_t> *messageQueue;
       queue<messageTransfer_t> *responseQueue;
       mutex *msgQueueLock;
       mutex *respQueueLock;
       counting_semaphore<1> *messageSemaphore;
       counting_semaphore<1> *responseSemaphore;

       static MessageQueue *instance;
       MessageQueue();

    public:
       ~MessageQueue();
       /* Singleton should not be cloneable */
       MessageQueue( MessageQueue &other ) = delete;
       /* Singleton should not be assignable */
       void operator=( const MessageQueue & ) = delete;
       static MessageQueue *getInstance();
       void addMessage( messageTransfer_t msg );
       void retrieveMessage( messageTransfer_t *msg);
       void addResponse( messageTransfer_t resp );
       void retrieveResponse( messageTransfer_t *resp );
};

#endif