

#include "ResponseQueue.h"

using namespace std;

ResponseQueue *
ResponseQueue::instance = nullptr;

ResponseQueue::ResponseQueue()
{
    responseQueue = new queue<messageTransfer_t>();
    queueLock = new mutex();
    semaphore = new counting_semaphore<1>(1);
}

ResponseQueue::~ResponseQueue()
{
    delete responseQueue;
    delete queueLock;
    delete semaphore;
}

void
ResponseQueue::addMessage( messageTransfer_t msg )
{
    queueLock->lock();
    {
        responseQueue->push( msg );
    }
    queueLock->unlock();

    /* Notify that something has been put on the queue. */
    semaphore->release();
}

void
ResponseQueue::retrieveMessage( messageTransfer_t *msg )
{

    /* Wait for something to be added to the queue. */
    semaphore->acquire();

    queueLock->lock();
    {
        if( !responseQueue->empty() )
        {
            msg->connection = responseQueue->front().connection;
            msg->buffer  = responseQueue->front().buffer;

            responseQueue->pop();
        }
    }
    queueLock->unlock();
}

ResponseQueue *
ResponseQueue::getInstance()
{
    if( instance == nullptr )
    {
        instance = new ResponseQueue();
    }

    return instance;
}