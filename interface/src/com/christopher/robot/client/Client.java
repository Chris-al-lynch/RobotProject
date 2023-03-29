package com.christopher.robot.client;

import java.io.OutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import com.christopher.robot.logger.Logger;
import com.christopher.robot.message.Message;
import com.christopher.robot.message.MessageException;
import com.christopher.robot.utilities.PrintUtilities;

public class Client
{
    private Logger logger;

    private String serverAddress;
    private int    serverPort;
    private Socket clientSocket;

    private OutputStream output;
    private InputStream  input;

    public Client( String serverAddress, int serverPort )
    {
        logger = new Logger();
        this.serverAddress = serverAddress;

        if( serverAddress == "localhost" )
        {
            this.serverAddress = "127.0.0.1";
        }

        this.serverPort = serverPort;

        clientSocket = null;
    }

    private void connect() throws ClientException
    {
        try
        {
            clientSocket = new Socket( serverAddress, serverPort );
        }
        catch( UnknownHostException uhe )
        {
            String message = "Invalid host was received: " + serverAddress
                           + " - " + uhe;
            logger.logError( message );
            throw new ClientException( message, uhe );
        }
        catch( IOException ioe )
        {
            String message = "Error Creating socket: " + ioe;
            logger.logError( message );
            throw new ClientException( message, ioe );
        }
        catch( SecurityException se )
        {
            String message = "Security violation...operation not allowed: " + se;
            logger.logError( message );
            throw new ClientException( message, se );
        }
        catch( IllegalArgumentException iae )
        {
            String message = "Invalid port received: " + serverPort + " - " + iae;
            logger.logError( message );
            throw new ClientException( message, iae );
        }

        if( clientSocket.isConnected() )
        {
            try
            {
                output = new DataOutputStream( clientSocket.getOutputStream() );
            }
            catch( IOException ioe )
            {
                throw new ClientException( "Failed to create DataOutputStream: " + ioe, ioe );
            }

            try
            {
                input  = new DataInputStream( clientSocket.getInputStream() );
            }
            catch( IOException ioe )
            {
                throw new ClientException( "Failed to create DataInputStream: " + ioe, ioe );
            }
        }
    }

    private void close() throws MessageException
    {
        try
        {
            if( output != null )
            {
                output.close();
            }

            if( input != null )
            {
                input.close();
            }

            if( clientSocket != null )
            {
                clientSocket.close();
            }
        }
        catch( IOException ioe )
        {
            throw new MessageException( "Failed to close properly: " + ioe, ioe );
        }
    }

    private void sendMessageToServer( byte[] message ) throws ClientException
    {

        String hex = "";

        System.out.println( "message size = " + message.length );

        PrintUtilities.printByteArray( message );

        System.out.println( "Message = \n" + hex );
        try
        {
            output.write( message );
        }
        catch( IOException ioe )
        {
            throw new ClientException( "Failed to write message to socket: " + ioe, ioe );
        }
    }

    private byte[] getResponse() throws ClientException
    {
        byte responseSizeBuffer[] = new byte[4];

        try
        {
            input.read( responseSizeBuffer, 0, 4 );
        }
        catch( IOException ioe )
        {
            throw new ClientException( "Failed to read response from socket: " + ioe, ioe );
        }

        ByteBuffer sizeWrapper = ByteBuffer.wrap( responseSizeBuffer.clone() );
        sizeWrapper.order( ByteOrder.LITTLE_ENDIAN );
        int responseSize       = sizeWrapper.getInt();

        byte responseBuffer[]      = new byte[responseSize];
        ByteBuffer responseWrapper = ByteBuffer.wrap( responseBuffer );
        sizeWrapper.order( ByteOrder.LITTLE_ENDIAN );
        responseWrapper.put( responseSizeBuffer );

        try
        {
            input.read( responseBuffer, Integer.BYTES, responseSize - Integer.BYTES );
        }
        catch( IOException ioe )
        {
            throw new ClientException( "Failed to read the response from the socket: " + ioe, ioe );
        }

        return responseBuffer;
    }

    public Message sendMessage( Message message ) throws ClientException, MessageException
    {
        logger.logInfo( "Connecting to server at " + serverAddress + ":" + serverPort );
        connect();
        logger.logInfo( "Sending message to server." );
        sendMessageToServer( message.pack() );
        logger.logInfo( "Waiting for response from server." );
        byte[] response = getResponse();
        logger.logInfo( "Closing connection." );
        close();

        System.out.println( "response length = " + response.length );
        System.out.println( "response = " );
        PrintUtilities.printByteArray( response );

        return Message.getMessageFromBuffer( response, ByteOrder.LITTLE_ENDIAN );
    }
}
