
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <fcntl.h>

#include <chrono>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <thread>

#include "Exceptions.h"
#include "Logger.h"
#include "Server.h"

using namespace std;

Server::Server( string listenAddress, int listenPort )
{
    status = STATUS_INITIALIZING;

    try
    {
        logger = new Logger( LOGGER_DEBUG );
        stopProcessing = false;
        tcpServer = new TcpServer( listenAddress, listenPort );

        messageQueue  = MessageQueue::getInstance();
        responseQueue = ResponseQueue::getInstance();

        /* Make sure everything is initialized before the threads are started. */
        responseThread = new jthread( &Server::processResponse, this );
        serverThread   = new jthread( &Server::server, this );
    }
    catch( exception& e )
    {
        status = STATUS_FAILED;
        throw e;
    }

    status = STATUS_FULLY_OPERATIONAL;
}

Server::~Server()
{
    shutdown();
}

void
Server::shutdown()
{
    delete tcpServer;
    delete responseQueue;
    delete responseThread;
    delete messageQueue;
    delete logger;
}

void
Server::server()
{
    try
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
        } while( true );
    }
    catch( exception& e )
    {
        status = STATUS_FAILED;
        logger->logError( "Server failed with: " + string( e.what() ) );
        shutdown();
        throw e;
    }
}

void
Server::dispatch( clientSocket_t connection, RawBuffer *msgBuffer )
{
    messageQueue->addMessage( messageTransfer_t{connection, msgBuffer} );
}

void
Server::processResponse()
{
    try
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
    catch( exception& e )
    {
        status = STATUS_FAILED;
        logger->logError( "Process Response failed with: " + string( e.what() ) );
        shutdown();
        throw e;
    }
}

Status
Server::getStatus()
{
    return status;
}
