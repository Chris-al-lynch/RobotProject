
#include <arpa/inet.h>
#include <cstring>

#include <unistd.h>

#include "Client.h"
#include "ClientException.h"
#include "Message.h"
#include "TestMessage.h"

Client::Client( string serverAddress, int serverPort )
              : serverAddress( serverAddress ), serverPort( serverPort )
{
    logger = new Logger( LOGGER_DEBUG );

    logger->logDebug( "Entered constructor" );

    clientSocket = -1;
    memset( &serverSocketAddress, 0, sizeof( struct sockaddr_in ) );
    logger->logDebug( "Cleared memory for serverSocketAddress" );
    serverSocketAddress.sin_family = AF_INET;
    serverSocketAddress.sin_port   = htons( serverPort );

    string hostname = serverAddress;

    if( serverAddress == "localhost" )
    {
        hostname = "127.0.0.1";
    }

    if( inet_pton( AF_INET, hostname.c_str(),
                   &serverSocketAddress.sin_addr ) <= 0 )
    {
        throw new ClientException( "Invalid address given: " + serverAddress );
    }

    logger->logDebug( "Exiting constructor." );
}

Client::~Client()
{
    if( clientSocket != -1 )
    {
        close( clientSocket );
    }
}
void
Client::initSocket()
{
    logger->logInfo( "Initializing socket." );

    clientSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    if( clientSocket < 0 )
    {
        throw ClientException( "Failed to create socket", errno );
    }
}

void
Client::connectToServer()
{
    int returnCode = connect( clientSocket, (sockaddr *)&serverSocketAddress,
                              sizeof( serverSocketAddress ) );

    if( returnCode < 0 )
    {
        throw ClientException( "Failed to make a connection", errno );
    }
}

int
Client::sendMessage( string messageToSend )
{
    char *unusedMsgBuffer = (char *)(messageToSend.c_str());
    TestMessage tmpMessage( unusedMsgBuffer  );

    char *msgBuffer = tmpMessage.generateTestMessage( messageToSend );
    TestMessage message( msgBuffer );

    logger->logInfo( "Sending message '" + messageToSend + "' to " + serverAddress + ":" + to_string( serverPort ) );
    //send( clientSocket, msgBuffer, ((int*)msgBuffer)[0], 0 );
    send( clientSocket, msgBuffer, message.getLength(), 0 );

    return message.getId();
}

void
Client::getTestResponse( string messageToSend, int msgId )
{
    printf( "Using id = %d\n", msgId );
    char *expectedRespBuffer = TestMessage::generateTestResponse( "Received message: "
                                                                + messageToSend, msgId );
    int responseBufferSize = (int)((int *)expectedRespBuffer)[0];
    char *actualRespBuffer = (char *)malloc( responseBufferSize );

    ssize_t bytesRead = recv( clientSocket, actualRespBuffer,
                              responseBufferSize, 0 );

    if( bytesRead != responseBufferSize )
    {
        throw ClientException( "Invalid response size: \nBytes Read: "
                             + to_string( bytesRead ) + "\nBytes Expected: "
                             + to_string( responseBufferSize ) );
    }

    TestMessage expected( expectedRespBuffer );
    TestMessage actual( actualRespBuffer );

    if( actual != expected )
    {
        throw ClientException( "Response Buffer does not match expected response buffer" );
    }

    logger->logInfo( "Received proper response." );
}

void
Client::sendTestMessage( string messageToSend )
{
    logger->logInfo( "Initializing the socket." );
    initSocket();
    logger->logInfo( "Connecting to the server." );
    connectToServer();
    logger->logInfo( "Sending message (" + messageToSend + ")to the server." );
    int msgId = sendMessage( messageToSend );
    logger->logInfo( "Getting response from the server." );
    getTestResponse( messageToSend, msgId );
    logger->logInfo( "Test complete." );
}
