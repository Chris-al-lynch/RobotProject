
#include "MessageQueue.h"

using namespace std;

MessageQueue *MessageQueue::instance;
once_flag MessageQueue::instanceFlag;

MessageQueue::MessageQueue()
{
    messageQueue = new queue<messageTransfer_t>();
    queueLock = new mutex();
    semaphore = new counting_semaphore<1>(1);
}

MessageQueue::~MessageQueue()
{
    delete messageQueue;
    delete queueLock;
    delete semaphore;
}

void
MessageQueue::addMessage( messageTransfer_t msg )
{
    queueLock->lock();
    {
        messageQueue->push( msg );
    }
    queueLock->unlock();

    /* Notify that something has been put on the queue. */
    semaphore->release();
}

void
MessageQueue::retrieveMessage( messageTransfer_t *msg )
{

    /* Wait for something to be added to the queue. */
    semaphore->acquire();

    queueLock->lock();
    {
        if( !messageQueue->empty() )
        {
            msg->connection = messageQueue->front().connection;
            msg->buffer  = messageQueue->front().buffer;

            messageQueue->pop();
        }
    }
    queueLock->unlock();
}

MessageQueue *
MessageQueue::getInstance()
{
    call_once( instanceFlag, [&]() { instance = new MessageQueue(); } );
    return instance;
}
