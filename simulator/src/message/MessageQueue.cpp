
#include "MessageQueue.h"

using namespace std;

MessageQueue *
MessageQueue::instance = nullptr;

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
    }
    msgQueueLock->unlock();

    /* Notify that something has been put on the queue. */
    messageSemaphore->release();
}

void
MessageQueue::retrieveMessage( messageTransfer_t *msg )
{

    /* Wait for something to be added to the queue. */
    messageSemaphore->acquire();

    msgQueueLock->lock();
    {
        if( !messageQueue->empty() )
        {
            msg->connection = messageQueue->front().connection;
            msg->msgBuffer  = messageQueue->front().msgBuffer;
            msg->respBuffer = nullptr;

            messageQueue->pop();
        }
    }
    msgQueueLock->unlock();
}

void
MessageQueue::addResponse( messageTransfer_t resp )
{
    respQueueLock->lock();
    {
        responseQueue->push( resp );
    }
    respQueueLock->unlock();

    /* Notify that something has been put on the queue. */
    responseSemaphore->release();
}

void
MessageQueue::retrieveResponse( messageTransfer_t *resp )
{
    /* Wait for something to be added to the queue. */
    responseSemaphore->acquire();

    respQueueLock->lock();
    {
        if( !responseQueue->empty() )
        {
            resp->connection = responseQueue->front().connection;
            resp->msgBuffer  = responseQueue->front().msgBuffer;
            resp->respBuffer = responseQueue->front().respBuffer;

            responseQueue->pop();
        }
    }
    respQueueLock->unlock();
}

MessageQueue *
MessageQueue::getInstance()
{
    if( instance == nullptr )
    {
        instance = new MessageQueue();
    }

    return instance;
}
