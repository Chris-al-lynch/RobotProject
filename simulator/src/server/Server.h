#ifndef _SERVER_H_
#define _SERVER_H_

#include <thread>

#include "Logger.h"
#include "MessageProcessor.h"
#include "MessageQueue.h"
#include "RawBuffer.h"
#include "TcpServer.h"

using namespace std;

/**
 * The robot needs a way to receive commands from the user.  This class is
 * that way.  It's job is to receive commands from the server and dispatch
 * the work.
 */
class Server
{
    private: 
       /* Object for performing TCP related tasks. */
       TcpServer *tcpServer;

       /* Queue to add messages for processing */
       MessageQueue *messageQueue;
       /* Queue for retrieving responses for processed messages. */
       ResponseQueue *responseQueue;

       /* Object for processing messages. */
       MessageProcessor *messageProcessor;

       /* Thread for monitoring the response queue and sending responses back
        * to the sender.
        */
       jthread *responseThread;

       /* I always create a logger class in case I want to change
        * my method of logging.
        */
       Logger *logger;

       /* Variable used to determine if we should stop processing.  The Stop
        * method will set this to true.
        */
       bool stopProcessing;

       /* Method for sending messages off to be processed. */
       void dispatch( clientSocket_t connection, RawBuffer *msgBuffer );

       /* Method for sending responses back to the sender. */
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
