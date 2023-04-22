
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <fcntl.h>

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

#include "Exceptions.h"
#include "Logger.h"
#include "Server.h"

using namespace std;

Server::Server( string listenAddress, int listenPort )
{
    logger = new Logger( LOGGER_DEBUG );
    stopProcessing = false;
    tcpServer = new TcpServer( listenAddress, listenPort );

    responseThread = new jthread( &Server::processResponse, this );
    messageProcessor = new MessageProcessor();
    messageQueue  = MessageQueue::getInstance();
    responseQueue = ResponseQueue::getInstance();
}

Server::~Server()
{
    delete logger;
    delete messageQueue;
    delete responseQueue;
    delete responseThread;
    delete tcpServer;
}

void
Server::start()
{
    logger->logInfo( "Initializing socket." );

    /* Initialize the socket. */
    tcpServer->initSocket();

    logger->logInfo( "Starting listener." );
    /* Start the listener. */
    tcpServer->startListener();

    /* As long as the server is running, continue to loop.  Wait for and process
     * new messages.
     */

    do
    {
        logger->logInfo( "Accepting connection." );

        /* Wait for a connection request from the client. Once the connection is
         * established, it will remain open until the message has been processed
         * and the response has been sent.
         */
        clientSocket_t connection = tcpServer->acceptConnection();

        logger->logInfo( "Retrieving message." );
        /* Retrieve the message from the client. */
        RawBuffer *msgBuffer = tcpServer->receiveRequest( connection );

        logger->logInfo( "Processing message." );
        /* Send the message to the worker to be processed.
         * The response will be processed by a separate thread.
         */
        dispatch( connection, msgBuffer );
    } while( !stopProcessing );
}

void
Server::stop()
{
    tcpServer->tcpClose();

    if( responseThread != nullptr )
    {
        responseThread->request_stop();
        responseThread->join();
    }

    stopProcessing = true;
}

void
Server::dispatch( clientSocket_t connection, RawBuffer *msgBuffer )
{
    messageQueue->addMessage( messageTransfer_t{connection, msgBuffer} );
}

void
Server::processResponse()
{
    do
    {
        logger->logInfo( "Waiting for a response to send." );
        messageTransfer_t response;

        /* Retrieve a response from the message queue. */
        responseQueue->retrieveMessage( &response );

        logger->logInfo( "Response received." );

        if( response.buffer != nullptr )
        {
            logger->logInfo( "Sending response." );
            /* Send the response back on the client connection. */
            tcpServer->sendResponse( response.connection, response.buffer );

            /* Response Sent...Free the buffer. */
            delete response.buffer;
        }
        else
        {
            logger->logInfo( "response buffer is NULL." );
        }

        /* Response sent. Close the connection. */
        close( response.connection );

    } while( true );
}
