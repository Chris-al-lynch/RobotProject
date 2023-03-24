
#include "MessageQueue.h"

using namespace std;

MessageQueue::MessageQueue()
{
    messageQueue = new queue<messageTransfer_t>();
    responseQueue = new queue<messageTransfer_t>();
    msgQueueLock = new mutex();
    respQueueLock = new mutex();
    messageSemaphore = new counting_semaphore<1>(1);
    responseSemaphore = new counting_semaphore<1>(1);
}

MessageQueue::~MessageQueue()
{
    delete messageQueue;
    delete responseQueue;
    delete msgQueueLock;
    delete respQueueLock;
    delete messageSemaphore;
    delete responseSemaphore;
}

void
MessageQueue::addMessage( messageTransfer_t msg )
{
    msgQueueLock->lock();
    {
        messageQueue->push( msg );
        /* Notify that something has been put on the queue. */
        messageSemaphore->release();
    }
    msgQueueLock->unlock();
}

messageTransfer_t
MessageQueue::retrieveMessage()
{
    messageTransfer_t msg;

    msgQueueLock->lock();
    {
        /* Wait for something to be added to the queue. */
        messageSemaphore->acquire();
        msg = messageQueue->front();
        messageQueue->pop();
    }
    msgQueueLock->unlock();

    return msg;
}

void
MessageQueue::addResponse( messageTransfer_t resp )
{
    respQueueLock->lock();
    {
        responseQueue->push( resp );
        /* Notify that something has been put on the queue. */
        responseSemaphore->release();
    }
    respQueueLock->unlock();
}

messageTransfer_t
MessageQueue::retrieveResponse()
{
    messageTransfer_t resp;

    respQueueLock->lock();
    {
        /* Wait for something to be added to the queue. */
        responseSemaphore->acquire();
        resp = responseQueue->front();
        responseQueue->pop();
    }
    respQueueLock->unlock();

    return resp;
}
