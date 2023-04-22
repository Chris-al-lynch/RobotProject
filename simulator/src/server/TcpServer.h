#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include <netinet/in.h>
#include <string>

#include "Logger.h"
#include "RawBuffer.h"

using namespace std;

enum commType
{
    READ,
    WRITE,
    EXCEPTION
};

typedef struct timeval timeval_t;
typedef int clientSocket_t;

class TcpServer
{
    private:

        /* Address on which to listen. */
        string listenAddress;
        /* Port on which to listen. */
        int listenPort;
        /* Socket file descriptor for connections. */
        int serverSocket;
        /* Socket Address */
        sockaddr_in socketAddress;

        Logger *logger;

        /**
         * Method for blocking because we are using a non-blocking socket.
         * FIXME: Select is not working how I remember.  I need to go back
         * and read the spec and figure out the optimal use for it.  Then,
         * I will fix this function.
         */
        void block( commType type, timeval_t *timeout );

    public:
        /**
         * Constructor.
         */
        TcpServer( string listenAddress, int listenPort );

        /**
         * Method to initialize the socket.
         */
        void initSocket();

        /**
         * Method to startng the listener.
         */
        void startListener();

        /**
         * Method that waits for a connection from a client.
         * 
         * @return client socket
         */
        clientSocket_t acceptConnection();

        /**
         * Method for retrieving messages from the client.
         * 
         * @param connection client socket.
         * @return RawBuffer containing the client message to be processed.
         */
        RawBuffer *receiveRequest( clientSocket_t connection );

        /**
         * Method for sending response back to the client.
         * 
         * @param connection client socket
         * @param response RawBuffer containing the response.
         */
        void sendResponse( clientSocket_t connection, RawBuffer *response );

        /*
         * Method to close the connection.
         */
        void tcpClose();
};

#endif