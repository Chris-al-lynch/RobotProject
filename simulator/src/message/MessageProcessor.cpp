
#include "Exceptions.h"
#include "Message.h"
#include "MessageProcessor.h"
#include "TestMessage.h"

MessageProcessor::MessageProcessor()
{
    logger = new Logger();
    processorThread = new jthread( MessageProcessor::processor, this );
    messageQueue = MessageQueue::getInstance();
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
    this_p->logger->logInfo( "Message processor thread started." );

    do
    {
        try
        {
            /* Wait for a message to be sent */
            messageTransfer_t message;
            memset( &message, 0, sizeof( message ) );
            this_p->messageQueue->retrieveMessage( &message );

            if( message.msgBuffer == nullptr )
            {
                close( message.connection );
                continue;
            }

            message.respBuffer = this_p->processMessage( message.msgBuffer );
            if( message.respBuffer != nullptr )
            {
                this_p->messageQueue->addResponse( message );
            }
            else
            {
                close( message.connection );
                free( message.msgBuffer );
            }
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
            logger->logInfo( "An unexpected message received." );
            break;
        }
    }

    return nullptr;
}