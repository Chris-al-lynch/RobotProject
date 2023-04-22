package com.christopher.robot.client;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import com.christopher.robot.logger.Logger;
import com.christopher.robot.message.Message;
import com.christopher.robot.message.MessageException;
import com.christopher.robot.message.Response;
import com.christopher.robot.message.ResponseFactory;

public class Client
{
    private Logger logger;

    private TcpClient tcpClient;

    public Client( String serverAddress, int serverPort )
    {
        logger = new Logger();
        tcpClient = new TcpClient( serverAddress, serverPort );
    }

    public Response sendMessage( Message message ) throws ClientException, MessageException
    {
        tcpClient.connect();

        ByteBuffer buffer = ByteBuffer.allocate( message.getLength() );
        buffer.order( ByteOrder.LITTLE_ENDIAN );
        message.pack( buffer );

        tcpClient.sendMessageToServer( buffer.array() );

        logger.logInfo( "Waiting for response from server." );
        byte[] response = tcpClient.getResponse();

        ByteBuffer responseBuffer = ByteBuffer.wrap( response );
        responseBuffer.order( ByteOrder.LITTLE_ENDIAN );

        logger.logInfo( "Closing connection." );
        tcpClient.close();

        return ResponseFactory.createResponse( responseBuffer );
    }
}
