
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
#include "MessageQueue.h"
#include "Server.h"

using namespace std;

Server::Server( string listenAddress, int listenPort )
              : listenAddress( listenAddress ), listenPort( listenPort )
{
    logger = new Logger( LOGGER_DEBUG );
    serverSocket = -1;
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = inet_addr( listenAddress.c_str() );
    socketAddress.sin_port = htons( listenPort );
    messageQueue = new MessageQueue();
    responseThread = new jthread( &Server::processResponse, this );
}

Server::~Server()
{
    delete logger;
    delete messageQueue;
    delete responseThread;
}

void
Server::start()
{
    /* Initialize the socket. */
    initSocket();
    /* Start the listener. */
    startListener();

    /* As long as the server is running, continue to loop wait for and process
     * new messages.
     */

    do
    {
        /* Wait for a connection request from the client. */
        clientSocket connection = acceptConnection();

        /* Retrieve the message from the client. */
        char *msgBuffer = receiveRequest( connection );

        /* Send the message to the worker to be processed.
         * The response will be processed by a separate thread.
         */
        dispatch( connection, msgBuffer );
    } while( true );
}

void
Server::initSocket()
{
    logger->logInfo( "Initializing Socket" );

    /* Create a TCP socket */
    serverSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    if( serverSocket < 0 )
    {
        throw ServerException( errno, "Failed to create socket" );
    }

    /* For handling timeouts, it's best to use non-blocking sockets. */
    /* Get the existing flags */
    int socketFlags = fcntl( serverSocket, F_GETFL );

    /* bitwise OR in the non-blocking flag */
    socketFlags |= O_NONBLOCK;

    /* Store the new flags. */
    fcntl( serverSocket, F_SETFL, socketFlags );

    /* Allow reuse of local addresses */
    int value = 1;
    if( setsockopt( serverSocket, SOL_SOCKET, SO_REUSEADDR, &value,
                    sizeof( value ) ) < 0 )
    {
        throw ServerException( errno, "Failed to set socket option REUSEADDR" );
    }

    /* Assign the socket to the address */
    if( bind( serverSocket, (struct sockaddr *)&socketAddress,
              sizeof( sockaddr ) ) < 0 )
    {
        throw ServerException( errno, "Failed to assign the socket to address "
                                    + listenAddress + ":" + to_string( listenPort ) );
    }

    logger->logInfo( "Socket initialized successfully" );
}

void
Server::startListener()
{
    logger->logInfo( "Starting the listener" );
    /* We only want to listen to one connection at a time.  We are
     * controlling a robot.  The robot could have issues with multiple
     * people attempting to control it at the same time.
     */
    if( listen( serverSocket, 1 ) < 0 )
    {
        throw ServerException( errno, "Failed to listen on socket" );
    }

    logger->logInfo( "Listener started successfully" );
}

void
Server::block( commType type, timeval_t *timeout )
{
    /* Since our socket is non-blocking, we must use select() to wait for a
     * connection request.  If we called accept and there was no connection
     * request, accept would fail with  EAGAIN or EWOULDBLOCK.
     * 
     * NOTE: I did some research to determine if I should use select(), poll(),
     * or something else.  The fact that there is only one file descriptor
     * in my use case says that select will work just fine.  Select is the
     * oldest, I have experience with select, and there is no additional
     * benefits using poll() with just one file descriptor.
     */

    /* Select is meant to deal with sets of file descriptors (or sockets).  I
     * only have one, but it still needs to be added to a set to be used by
     * select.
     */
    fd_set socketSet;

    /* Initialize the set: clear the memory. */
    FD_ZERO( &socketSet );

    /* Add my socket to the set. */
    FD_SET( serverSocket, &socketSet );

    switch( type )
    {
        case READ:
            /* Select will block until ready for read or timeout occurs. */
            if( select( serverSocket + 1, &socketSet, NULL, NULL, timeout ) < 0 )
            {
                throw ServerException( errno, "select failed waiting for accept" );
            }
            break;
        case WRITE:
            /* Select will block until ready for writing or timeout occurs. */
            if( select( serverSocket + 1, NULL, &socketSet, NULL, timeout ) < 0 )
            {
                throw ServerException( errno, "select failed waiting for accept" );
            }
            break;
        case EXCEPTION:
            /* Select will block until a communication event or timeout occurs. */
            if( select( serverSocket + 1, NULL, NULL, &socketSet, timeout ) < 0 )
            {
                throw ServerException( errno, "select failed waiting for accept" );
            }
            break;
        default:
            throw ServerException( "Invalid parameter for block: " + to_string( type ) );
    }

    /* This if-statement should never actually be entered.  There is only 1
     * socket and no timeout.  If select returns, it should only be because
     * we received a exception event.
     */
    if( !FD_ISSET( serverSocket, &socketSet ) )
    {
        throw ServerException( "Unexpected behavior from select" );
    }
}

Server::clientSocket
Server::acceptConnection()
{
    logger->logInfo( "Waiting for connection request" );

    size_t sockaddrLen = sizeof( socketAddress );

    /* Block forever for except. */
    block( EXCEPTION, NULL );

    /* At this point, there should be a connection request, so call accept to
     * accept the request.
     */
    int connection = accept( serverSocket, (struct sockaddr *)&socketAddress,
                                 (socklen_t *)&sockaddrLen );

    if( connection < 0 )
    {
        throw ServerException( errno, "Failed to make connection" );
    }

    /* Return the client connection back to the caller. */
    return (clientSocket)connection;
}

char *
Server::receiveRequest( clientSocket connection )
{
    logger->logInfo( "Receiving data." );

    timeval_t timeout = { 3, 0 };

    ssize_t bytesRead;
    ssize_t totalBytes = 0;

    /* The first two bytes of the message is the size of the message. */
    int recvDataLength;

    do
    {
        /* Wait until there is data on the line to read. We were told something
         * was coming with the accept call, so it shouldn't take very long for
         * it to arrive.  Only wait 3 seconds.
         */
        block( READ, &timeout );

        /* Read the first two bytes of the message. Could use read() here, but
         * I like to use recv() when transferring messages in case I need to
         * use extra flags for the transfer.
         */
        bytesRead = recv( connection, (char *)&recvDataLength, 2, 0 );

        totalBytes += bytesRead;

        if( bytesRead < 2 )
        {
            if( bytesRead < 0 )
            {
                throw ServerException( errno, "recv() failed for length" );
            }

            if( bytesRead == 0 )
            {
                throw LostConnectionException( "Connection was closed unexpectedly while reading length." );
            }
        }
    } while( totalBytes < 2 );

    char *msgBuffer = (char *)calloc( recvDataLength, sizeof( char ) );

    if( msgBuffer == NULL )
    {
        logger->logError( "calloc() failed to allocate buffer. There may be a memory leak or something wrong with the system." );
        throw bad_alloc();
    }

    /* The size of the message includes the two bytes just read for the length.
     * We need to add the length back into the buffer so that the message buffer
     * is whole for the message handler.
     */
    memcpy( msgBuffer, &recvDataLength, 2 );

    totalBytes = 0;

    /* Now read the data. */
    do
    {
        /* Wait until there is data on the line to read. We were told something
         * was coming with the accept call, so it shouldn't take very long for
         * it to arrive.  Only wait 3 seconds.
         */
        block( READ, &timeout );

        /* Read the first two bytes of the message. Could use read() here, but
         * I like to use recv() when transferring messages in case I need to
         * use extra flags for the transfer.
         */
        bytesRead = recv( connection, (char *)msgBuffer, recvDataLength, 0 );

        totalBytes += bytesRead;

        if( bytesRead < 0 )
        {
            throw ServerException( errno, "recv() failed for data" );
        }

        if( bytesRead == 0 )
        {
            throw LostConnectionException( "Connection was closed unexpectedly while reading data." );
        }
    } while( totalBytes < bytesRead );

    return msgBuffer;
}

void
Server::stop()
{
    if( serverSocket != -1 )
    {
        close( serverSocket );
    }

    if( responseThread != NULL )
    {
        responseThread->request_stop();
        responseThread->join();
    }

    if( messageQueue != NULL )
    {

    }
}

void
Server::dispatch( clientSocket connection, char *msgBuffer )
{
    messageQueue->addMessage( {connection, msgBuffer, NULL} );
}

void
Server::processResponse()
{
    do
    {
        messageTransfer_t response = messageQueue->retrieveResponse();

        if( response.respBuffer != NULL )
        {
            sendResponse( response.connection, response.respBuffer );
            free( response.respBuffer );
        }

        if( response.msgBuffer != NULL )
        {
            free( response.msgBuffer );
        }

        close( response.connection );

    } while( true );
}

void
Server::sendResponse( clientSocket connection, char *respBuffer )
{
    timeval_t timeout = {3, 0};
    ssize_t bytesSent;
    ssize_t totalBytes = 0;

    /* I'm playing games with pointers here.  Probably not the safest thing
     * to do, but it works.  The first two byte is the length of the entire
     * response buffer.  I cast the reponse buffer to a pointer to an int
     * instead of a char.  Then I look at the first int in the array of ints
     * to get the length of the reponse buffer.
     */
    ssize_t respBufferLength = ((int *)respBuffer)[0];

    do
    {
        block( WRITE, &timeout );

        bytesSent = send( connection, respBuffer, respBufferLength, 0 );

        if( bytesSent < 0 )
        {
            throw ServerException( errno, "Failed to send response to client." );
        }

        if( bytesSent == 0 )
        {
            throw LostConnectionException( "Connection was closed by client before receiving response." );
        }

        totalBytes += bytesSent;

    } while( totalBytes < respBufferLength );
}
