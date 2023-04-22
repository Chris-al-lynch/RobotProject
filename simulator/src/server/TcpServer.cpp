
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#include "Exceptions.h"
#include "TcpServer.h"

TcpServer::TcpServer( string listenAddress, int listenPort )
                 : listenAddress( listenAddress ), listenPort( listenPort )
{
    logger = new Logger();

    serverSocket = -1;

    string hostname = listenAddress;

    /* FIXME: Need to use the correct functions for handling hostname.
     *        this is just a work-around for now.
     */
    if( listenAddress == "localhost" )
    {
        hostname = "127.0.0.1";
    }

    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = inet_addr( hostname.c_str() );
    socketAddress.sin_port = htons( listenPort );
}

void
TcpServer::initSocket()
{
    /* Create a TCP socket */
    serverSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    if( serverSocket < 0 )
    {
        throw TcpException( "Failed to create socket", errno );
    }

    /* For handling timeouts, it's best to use non-blocking sockets.
     * The following code is how you set a socket for non-blocking.
     */
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
        throw TcpException( "Failed to set socket option REUSEADDR", errno );
    }

    /* Assign the socket to the address */
    if( bind( serverSocket, (struct sockaddr *)&socketAddress,
              sizeof( sockaddr ) ) < 0 )
    {
        throw TcpException( "Failed to assign the socket to address "
                             + listenAddress + ":" + to_string( listenPort ),
                               errno );
    }

    logger->logInfo( "Socket initialized successfully" );
}

void
TcpServer::startListener()
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

clientSocket_t
TcpServer::acceptConnection()
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
    return (clientSocket_t)connection;
}

RawBuffer *
TcpServer::receiveRequest( clientSocket_t connection )
{
    logger->logInfo( "Receiving data." );

    /* Set the timeout for 3 seconds. */
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
         * 
         * FIXME: select is not working how I remember.  Look up the documentation
         *        and figure this out.
         */
        //block( READ, &timeout );

        /* Read the first two bytes of the message. Could use read() here, but
         * I like to use recv() when transferring messages in case I need to
         * use extra flags for the transfer.
         */
        bytesRead = recv( connection, (char *)&recvDataLength, sizeof( int ), 0 );

        totalBytes += bytesRead;

        /* Look for errors.
         * < 0 means we got an error.
         * = 0 means we lost the connections.
         * > 0 but < 2 means we have another byte to read.
         */
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

    /* Initialize the total bytes to the int we already read. */
    totalBytes = sizeof( int );

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

        /* Add to the totalbytes read.  Remember that recv() may not receive
         * all of the data at once.
         */
        totalBytes += bytesRead;

        /* If bytes read < 0, we got an error. */
        if( bytesRead < 0 )
        {
            throw ServerException( "recv() failed for data", errno );
        }

        /* If bytes read == 0, we lost connection. */
        if( bytesRead == 0 )
        {
            throw LostConnectionException( "Connection was closed unexpectedly while reading data." );
        }
      /* Loop as long as there is more data to read. */
      /* TODO: May want to add a timeout if we are taking too long to read the data. */
    } while( totalBytes < recvDataLength );

    /* Put the message into a RawBuffer. */
    /* TODO: add a wrap method to the RawBuffer class.  A wrap method would
     * not copy the msgBuffer, but simply store the pointer so we don't have
     * to copy the entire buffer.
     */
    RawBuffer *newBuffer = new RawBuffer( msgBuffer, recvDataLength );

    /* For now, free the msgBuffer.  It is no longer being used.  See the TODO above. */
    free( msgBuffer );

    return newBuffer;
}

void
TcpServer::sendResponse( clientSocket_t connection, RawBuffer *respBuffer )
{
    timeval_t timeout = {3, 0};
    ssize_t bytesSent;
    ssize_t totalBytes = 0;

    ssize_t respBufferLength = respBuffer->getInt( 0 );

    do
    {
        /* TODO: Study select code and figure out what's going on here. */
        //block( WRITE, &timeout );

        /* Send the data to the client. The send() function may not send all of the
         * data on the first call.  You have to call send() again until all of the
         * data is sent.
         * TODO: I'm not convinced this code is correct.  I need to reduce the IP buffer
         * size to something smaller than my message size so I can test when not all the
         * data is sent at once.  I think I'm supposed to send from an offset based on
         * the number of bytes sent. FIXME: Fix this code.
         */
        bytesSent = send( connection, respBuffer->getBuffer(), respBufferLength, 0 );

        /* If bytesSent < 0 an error occurred. */
        if( bytesSent < 0 )
        {
            throw ServerException( "Failed to send response to client.", errno );
        }

        /* If bytesSent == 0, we lost the connection. */
        if( bytesSent == 0 )
        {
            throw LostConnectionException( "Connection was closed by client before receiving response." );
        }

        totalBytes += bytesSent;

      /* Loop until all of the data is sent. */
    } while( totalBytes < respBufferLength );
}

/* FIXME: Lookup select spec and figure out what this is not working like I remember.
 * Comment this function after everything is finished.
 */
#define TEST
#ifdef TEST
void
TcpServer::block( commType type, timeval_t *timeout )
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
TcpServer::block( commType type, timeval_t *timeout )
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

void
TcpServer::tcpClose()
{
    if( serverSocket != -1 )
    {
        /* Close the connection. */
        close( serverSocket );
    }
}
