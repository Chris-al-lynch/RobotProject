#include <iostream>

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
        messageTransfer_t message;
        memset( &message, 0, sizeof( message ) );
        message.connection = -1;

        try
        {
            /* Wait for a message to be sent */
            this_p->messageQueue->retrieveMessage( &message );

            if( message.msgBuffer == nullptr )
            {
                close( message.connection );
                continue;
            }

            message.respBuffer = this_p->processMessage( message.msgBuffer );
            if( message.respBuffer != nullptr )
            {
                cout << "Adding reponse to message queue." << endl;
                this_p->messageQueue->addResponse( message );
            }
            else
            {
                cout << "No response buffer, closing connection." << endl;
                close( message.connection );
                message.connection = -1;

                if( message.msgBuffer != nullptr )
                {
                    free( message.msgBuffer );
                    message.msgBuffer = nullptr;
                }
            }
        }
        catch( const exception& e )
        {
            if( message.connection != -1 )
            {
                close( message.connection );
            }

            if( message.msgBuffer != nullptr )
            {
                free( message.msgBuffer );
            }

            this_p->logger->logError( "A failure occurred while processing a message: "
                                    + string( e.what() ) );
        }
    } while( true );
}

char *
MessageProcessor::processMessage( char *messageBuffer )
{
    Message message( messageBuffer );

    cout << "Called MessageProcessor::processMessage" << endl;

    if( !message.isMessageValid() )
    {
        throw MessageException( "Invalid message received" );
    }

    switch( message.getType() )
    {
        case TEST_MESSAGE:
        {
            cout << "have a test message." << endl;
            TestMessage tm( messageBuffer );
            logger->logInfo( "Test Message received with id = "
                           + to_string( tm.getId() ) );
            return tm.processMessage();
        }
        case MOVEMENT_MESSAGE:
        {
            cout << "have a movement message." << endl;
            break;
        }
        case SENSE_MESSAGE:
        {
            cout << "have a sense message." << endl;
            break;
        }
        default:
        {
            cout << "have an unexpected message." << endl;
            logger->logInfo( "An unexpected message received." );
            break;
        }
    }

    return nullptr;
}