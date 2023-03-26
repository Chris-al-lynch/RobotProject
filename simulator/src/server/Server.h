#ifndef _SERVER_H_
#define _SERVER_H_

#include <netinet/in.h>

#include <thread>

#include "Logger.h"
#include "MessageProcessor.h"
#include "MessageQueue.h"

using namespace std;

typedef enum commType
{
    READ,
    WRITE,
    EXCEPTION
} commType;

/**
 * The robot needs a way to receive commands from the user.  This class is
 * that way.  It's job is to receive commands from the server and dispatch
 * the work.
 */
class Server
{
    private: 
       typedef int clientSocket;
       typedef struct timeval timeval_t;

       /* Address on which to listen. */
       string listenAddress;
       /* Port on which to listen. */
       int listenPort;
       /* Socket file descriptor. */
       int serverSocket;
       /* Socket Address */
       sockaddr_in socketAddress;
       /* Message Queue */
       MessageQueue *messageQueue;
       MessageProcessor *messageProcessor;
       jthread *responseThread;

       /* I always create a logger class in case I want to change
        * my method of logging.
        */
       Logger *logger;

       /* Queue the work. */
       void dispatch( clientSocket connection, char *msgBuffer );

       void initSocket();
       void startListener();
       void block( commType type, timeval_t *timeout );
       clientSocket acceptConnection();
       char *receiveRequest( clientSocket connection );
       void sendResponse( clientSocket connection, char *response );
       void processResponse();

    public:
        /**
         * Server constructor
         */
        Server( string listenAddress, int listenPort );
        /**
         * Server destructor.
         */
        ~Server();
        /**
         * Start the server.
         */
        void start();
        /**
         * Stop the server.
         */
        void stop();
};

#endif
