
#include "Exceptions.h"
#include "Message.h"
#include "MessageProcessor.h"
#include "TestMessage.h"

MessageProcessor::MessageProcessor()
{
    logger = new Logger();
    processorThread = new jthread( MessageProcessor::processor, this );
    messageQueue = new MessageQueue();
}

void
MessageProcessor::start()
{
    delete processorThread;
    delete messageQueue;
}

void
MessageProcessor::processor( MessageProcessor *this_p )
{
    do
    {
        try
        {
            /* Wait for a message to be sent */
            messageTransfer_t message = this_p->messageQueue->retrieveMessage();
            message.respBuffer = this_p->processMessage( message.msgBuffer );
            this_p->messageQueue->addResponse( message );
        }
        catch( const exception& e )
        {
            this_p->logger->logError( "A failure occurred while processing a message: "
                                    + string( e.what() ) );
        }
    } while( true );
}

char *
MessageProcessor::processMessage( char *messageBuffer )
{
    Message message( messageBuffer );

    if( !message.isMessageValid() )
    {
        throw MessageException( "Invalid message received" );
    }

    switch( message.getType() )
    {
        case TEST_MESSAGE:
        {
            TestMessage tm( messageBuffer );
            logger->logInfo( "Test Message received with id = "
                           + to_string( tm.getId() ) );
            return tm.processMessage();
            break;
        }
        case MOVEMENT_MESSAGE:
        {
            break;
        }
        case SENSE_MESSAGE:
        {
            break;
        }
        case RESPONSE_MESSAGE:
        {
            break;
        }
        default:
        {
            break;
        }
    }

    return NULL;
}