#ifndef __MESSAGE_QUEUE_INTERFACE_H__
#define __MESSAGE_QUEUE_INTERFACE_H__

#include "RawBuffer.h"

typedef struct messageTransfer
{
    int connection;
    RawBuffer *buffer;
} messageTransfer_t;

class MessageQueueInterface
{
    public:
        virtual void addMessage( messageTransfer_t msg ) = 0;
        virtual void retrieveMessage( messageTransfer_t *msg ) = 0;
};
#endif
