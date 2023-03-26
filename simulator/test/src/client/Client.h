#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <netinet/in.h>
#include <string>

#include "Logger.h"

using namespace std;

class Client
{
    private:
        Logger *logger;
        string serverAddress;
        int serverPort;
        int clientSocket;
        struct sockaddr_in serverSocketAddress;

        void initSocket();
        void connectToServer();
        int sendMessage( string messageToSend );
        
      
    public:
        Client( string serverAddress, int serverPort );
        ~Client();
        void sendTestMessage( string messageToSend );
        void getTestResponse( string messageToSend, int msgId );
};

#endif