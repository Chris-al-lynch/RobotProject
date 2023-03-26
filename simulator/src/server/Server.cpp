
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
              : listenAddress( listenAddress ), listenPort( listenPort )
{
    logger = new Logger( LOGGER_DEBUG );
    serverSocket = -1;

    string hostname = listenAddress;
    if( listenAddress == "localhost" )
    {
        hostname = "127.0.0.1";
    }
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = inet_addr( hostname.c_str() );
    socketAddress.sin_port = htons( listenPort );

    responseThread = new jthread( &Server::processResponse, this );
    messageProcessor = new MessageProcessor();
    messageQueue = MessageQueue::getInstance();
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
    logger->logInfo( "Initializing socket." );
    /* Initialize the socket. */
    initSocket();
    logger->logInfo( "Starting listener." );
    /* Start the listener. */
    startListener();

    /* As long as the server is running, continue to loop wait for and process
     * new messages.
     */

    do
    {
        logger->logInfo( "Accepting connection." );
        /* Wait for a connection request from the client. */
        clientSocket connection = acceptConnection();

        logger->logInfo( "Retrieving message." );
        /* Retrieve the message from the client. */
        char *msgBuffer = receiveRequest( connection );

        logger->logInfo( "Processing message." );
        /* Send the message to the worker to be processed.
         * The response will be processed by a separate thread.
         */
        dispatch( connection, msgBuffer );
    } while( true );
}

void
Server::initSocket()
{
    /* Create a TCP socket */
    serverSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    if( serverSocket < 0 )
    {
        throw ServerException( "Failed to create socket", errno );
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
        throw ServerException( "Failed to set socket option REUSEADDR", errno );
    }

    /* Assign the socket to the address */
    if( bind( serverSocket, (struct sockaddr *)&socketAddress,
              sizeof( sockaddr ) ) < 0 )
    {
        throw ServerException( "Failed to assign the socket to address "
                             + listenAddress + ":" + to_string( listenPort ),
                               errno );
    }

    logger->logInfo( "Socket initialized successfully" );
}

void
Server::startListener()
{
    /* We only want to listen to one connection at a time.  We are
     * controlling a robot.  The robot could have issues with multiple
     * people attempting to control it at the same time.
     */
    if( listen( serverSocket, 1 ) < 0 )
    {
        throw ServerException( "Failed to listen on socket", errno );
    }
}

#define TEST
#ifdef TEST
void
Server::block( commType type, timeval_t *timeout )
{
    fd_set readSocketSet;
    FD_ZERO( &readSocketSet );
    FD_SET( serverSocket, &readSocketSet );

    fd_set writeSocketSet;
    FD_ZERO( &writeSocketSet );
    FD_SET( serverSocket, &writeSocketSet );

    fd_set exceptionSocketSet;
    FD_ZERO( &exceptionSocketSet );
    FD_SET( serverSocket, &exceptionSocketSet );

    /* Select will block until ready for read or timeout occurs. */
    if( select( serverSocket + 1, &readSocketSet, &writeSocketSet, &exceptionSocketSet, timeout ) < 0 )
    {
        throw ServerException( "select failed waiting for accept", errno );
    }

    logger->logInfo( "Expecting a " + string( ((type == READ) ? " READ " : ((type == WRITE) ? "WRITE" : "EXCEPTION")) ) + " select." );

    /* This if-statement should never actually be entered.  There is only 1
     * socket and no timeout.  If select returns, it should only be because
     * we received a exception event.
     */
    if( FD_ISSET( serverSocket, &readSocketSet ) )
    {
        logger->logInfo( "Received a read select." );
    }
    else if( FD_ISSET( serverSocket, &writeSocketSet ) )
    {
        logger->logInfo( "Received a write select." );
    }
    else if( FD_ISSET( serverSocket, &exceptionSocketSet ) )
    {
        logger->logInfo( "Received a exception select." );
    }
    else
    {
        throw ServerException( "Unexpected behavior from select" );
    }
}
#else
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
            if( select( serverSocket + 1, &socketSet, nullptr, nullptr, timeout ) < 0 )
            {
                throw ServerException( "select failed waiting for accept", errno );
            }
            break;
        case WRITE:
            /* Select will block until ready for writing or timeout occurs. */
            if( select( serverSocket + 1, nullptr, &socketSet, nullptr, timeout ) < 0 )
            {
                throw ServerException( "select failed waiting for accept", errno );
            }
            break;
        case EXCEPTION:
            /* Select will block until a communication event or timeout occurs. */
            if( select( serverSocket + 1, nullptr, nullptr, &socketSet, timeout ) < 0 )
            {
                throw ServerException( "select failed waiting for accept", errno );
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
#endif

Server::clientSocket
Server::acceptConnection()
{
    size_t sockaddrLen = sizeof( socketAddress );

    /* Block forever for except. */
    block( EXCEPTION, nullptr );

    /* At this point, there should be a connection request, so call accept to
     * accept the request.
     */
    int connection = accept( serverSocket, (struct sockaddr *)&socketAddress,
                                 (socklen_t *)&sockaddrLen );

    if( connection < 0 )
    {
        throw ServerException( "Failed to make connection", errno );
    }

    /* Return the client connection back to the caller. */
    return (clientSocket)connection;
}

#undef TEST
#ifdef TEST
char *
Server::receiveRequest( clientSocket connection )
{
    ssize_t bytesRead;
    int recvDataLength;
    char *msgBuffer = (char *)calloc( 1024, sizeof( char ) );

    bytesRead = recv( connection, msgBuffer, 1024, 0 );

    if( bytesRead <= 0 )
    {
        throw ServerException( "Failed to receive data length", errno );
    }

    logger->logInfo( "Read " + to_string( bytesRead ) + " bytes of data." );


/*
    logger->logInfo( "Reading data length" );
    bytesRead = recv( connection, (char *)&recvDataLength, sizeof( int ), MSG_DONTWAIT );

    if( bytesRead < 2 )
    {
        throw ServerException( "Failed to receive data length", errno );
    }

    logger->logInfo( "Data length is " + to_string( recvDataLength ) );

    char *msgBuffer = (char *)calloc( recvDataLength, sizeof( char ) );

    memcpy( msgBuffer, &recvDataLength, sizeof( int ) );

    logger->logInfo( "Reading data" );
    bytesRead = recv( connection, &msgBuffer[sizeof( int )], recvDataLength - sizeof( int ), MSG_DONTWAIT );

    if( bytesRead < 2 )
    {
        throw ServerException( "Failed to receive data", errno );
    }

    logger->logInfo( "Read " + to_string( bytesRead ) + " bytes of data." );
    */

    return msgBuffer;
}

#else
char *
Server::receiveRequest( clientSocket connection )
{
    logger->logInfo( "Receiving data." );

    timeval_t timeout = { 3, 0 };

    ssize_t bytesRead;
    ssize_t totalBytes = 0;

    /* The first two bytes of the message is the size of the message. */
    int recvDataLength;

    logger->logInfo( "Reading data length." );

    do
    {
        /* Wait until there is data on the line to read. We were told something
         * was coming with the accept call, so it shouldn't take very long for
         * it to arrive.  Only wait 3 seconds.
         */
        //block( READ, &timeout );

        /* Read the first two bytes of the message. Could use read() here, but
         * I like to use recv() when transferring messages in case I need to
         * use extra flags for the transfer.
         */
        bytesRead = recv( connection, (char *)&recvDataLength, sizeof( int ), 0 );

        totalBytes += bytesRead;

        if( bytesRead < 2 )
        {
            if( bytesRead < 0 )
            {
                throw ServerException( "recv() failed for length", errno  );
            }

            if( bytesRead == 0 )
            {
                throw LostConnectionException( "Connection was closed unexpectedly while reading length." );
            }
        }
    } while( totalBytes < 2 );
    logger->logInfo( "Read a total of " + to_string( totalBytes ) + " bytes when reading length." );

    logger->logInfo( "Allocating buffer that is " + to_string( recvDataLength ) + " bytes." );
    char *msgBuffer = (char *)calloc( recvDataLength, sizeof( char ) );

    if( msgBuffer == nullptr )
    {
        logger->logError( "calloc() failed to allocate buffer. There may be a memory leak or something wrong with the system." );
        throw bad_alloc();
    }

    /* The size of the message includes the two bytes just read for the length.
     * We need to add the length back into the buffer so that the message buffer
     * is whole for the message handler.
     */
    memcpy( msgBuffer, &recvDataLength, sizeof( int ) );

    totalBytes = sizeof( int );

    logger->logInfo( "Reading " + to_string( recvDataLength ) + " bytes of data." );

    /* Now read the data. */
    do
    {
        /* Wait until there is data on the line to read. We were told something
         * was coming with the accept call, so it shouldn't take very long for
         * it to arrive.  Only wait 3 seconds.
         */
        //block( READ, &timeout );

        /* Read the first two bytes of the message. Could use read() here, but
         * I like to use recv() when transferring messages in case I need to
         * use extra flags for the transfer.
         */
        bytesRead = recv( connection, &msgBuffer[totalBytes], recvDataLength - totalBytes, 0 );

        logger->logInfo( "Received " + to_string( bytesRead ) + " more bytes of data." );

        totalBytes += bytesRead;

        if( bytesRead < 0 )
        {
            throw ServerException( "recv() failed for data", errno );
        }

        if( bytesRead == 0 )
        {
            throw LostConnectionException( "Connection was closed unexpectedly while reading data." );
        }
    } while( totalBytes < recvDataLength );

    logger->logInfo( "Retrieved " + to_string( totalBytes ) + " of data." );

    return msgBuffer;
}
#endif

void
Server::stop()
{
    if( serverSocket != -1 )
    {
        close( serverSocket );
    }

    if( responseThread != nullptr )
    {
        responseThread->request_stop();
        responseThread->join();
    }

    if( messageQueue != nullptr )
    {

    }
}

void
Server::dispatch( clientSocket connection, char *msgBuffer )
{
    messageQueue->addMessage( messageTransfer_t{connection, msgBuffer, nullptr} );
}

void
Server::processResponse()
{
    do
    {
        logger->logInfo( "Waiting for a response to send." );
        messageTransfer_t response;
        messageQueue->retrieveResponse( &response );

        logger->logInfo( "Response received." );

        if( response.respBuffer != nullptr )
        {
            logger->logInfo( "Sending response." );
            sendResponse( response.connection, response.respBuffer );
            free( response.respBuffer );
        }
        else
        {
            logger->logInfo( "response buffer is NULL." );
        }

        if( response.msgBuffer != nullptr )
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
        //block( WRITE, &timeout );

        bytesSent = send( connection, respBuffer, respBufferLength, 0 );

        if( bytesSent < 0 )
        {
            throw ServerException( "Failed to send response to client.", errno );
        }

        if( bytesSent == 0 )
        {
            throw LostConnectionException( "Connection was closed by client before receiving response." );
        }

        totalBytes += bytesSent;

    } while( totalBytes < respBufferLength );
}
