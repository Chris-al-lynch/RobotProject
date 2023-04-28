#ifndef _SERVER_H_
#define _SERVER_H_

#include <future>
#include <thread>

#include "Logger.h"
#include "MessageProcessor.h"
#include "MessageQueue.h"
#include "RawBuffer.h"
#include "Status.h"
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

        /* Server Task */
        packaged_task<void()> *serverTask;

        /* Thread for waiting for messages from the client. */
        jthread *serverThread;

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

        Status status;

        /* Method for sending messages off to be processed. */
        void dispatch( clientSocket_t connection, RawBuffer *msgBuffer );

        /* Method for sending responses back to the sender. */
        void processResponse();

        /* Method for waiting for messages from the client. */
        void server();

        void shutdown();

    public:
        /**
         * Server constructor
         */
        Server( string listenAddress, int listenPort );
        /**
         * Server destructor.
         */
        ~Server();

        Status getStatus();
};

#endif
