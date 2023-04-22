
#include "Exceptions.h"
#include "Message.h"
#include "MessageFactory.h"
#include "MessageHeader.h"
#include "MessageProcessor.h"
#include "MessageTrailer.h"
#include "TestMessage.h"

using namespace std;

MessageProcessor::MessageProcessor()
{
    logger = new Logger();
    processorThread = new jthread( MessageProcessor::processor, this );
    messageQueue = MessageQueue::getInstance();
    responseQueue = ResponseQueue::getInstance();
}


MessageProcessor::~MessageProcessor()
{
    delete processorThread;
    delete messageQueue;
    delete responseQueue;
    delete logger;
}

void
MessageProcessor::processor( MessageProcessor *this_p )
{
    this_p->logger->logInfo( "Message processor thread started." );

    do
    {
        messageTransfer_t message;
        memset( &message, 0, sizeof( message ) );
        message.connection = -1;

        try
        {
            /* Wait for a message to be sent */
            this_p->messageQueue->retrieveMessage( &message );

            if( message.buffer == nullptr )
            {
                close( message.connection );
                continue;
            }

            //this_p->logger->logInfo( message.buffer->toString() );

            message.buffer = this_p->processMessage( message.buffer );

            if( message.buffer != nullptr )
            {
                this_p->responseQueue->addMessage( message );
            }
            else
            {
                close( message.connection );
                message.connection = -1;
            }
        }
        catch( const exception& e )
        {
            if( message.connection != -1 )
            {
                close( message.connection );
            }

            if( message.buffer != nullptr )
            {
                free( message.buffer );
            }

            this_p->logger->logError( "A failure occurred while processing a message: "
                                    + string( e.what() ) );
        }
    } while( true );
}

RawBuffer *
MessageProcessor::processMessage( RawBuffer *messageBuffer )
{
    Message  *message  = MessageFactory::unpack( messageBuffer );
    Response *response = message->processMessage();

    RawBuffer *responseBuffer = new RawBuffer( response->getHeader()->getLength() );
    response->pack( responseBuffer );

    delete message;
    delete response;

    return responseBuffer;
}