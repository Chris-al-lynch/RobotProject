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

    public:
       MessageQueue();
       ~MessageQueue();
       void addMessage( messageTransfer_t msg );
       messageTransfer_t retrieveMessage();
       void addResponse( messageTransfer_t resp );
       messageTransfer_t retrieveResponse();
};

#endif