package com.christopher.robot.message;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.Charset;

import com.christopher.robot.utilities.PrintUtilities;

public class TestMessage extends Message
{
    private MessageHeader  header;
    private String         message;
    private MessageTrailer trailer;
    private int            size;

    protected void changeType()
    {
        StackTraceElement ste = Thread.currentThread().getStackTrace()[2];
        if( ste.getClassName() == "TestResponseMessage" )
        {
            header.chageType( MessageType.TEST_RESPONSE );
        }
    }

    public TestMessage( String message )
    {
        this.message = message;
        size = MessageHeader.getSize() + MessageTrailer.getSize()
             + message.length();

        header = new MessageHeader( size, MessageType.TEST_MESSAGE, getMessageId() );
        trailer = new MessageTrailer();
    }

    public TestMessage( byte[] buffer, ByteOrder order ) throws MessageException
    {
        ByteBuffer bBuffer = ByteBuffer.wrap( buffer );
        bBuffer.order( order );
        header = MessageHeader.getHeaderFromBuffer( bBuffer );

        int stringSize = header.getMsgLength()
                       - MessageHeader.getSize()
                       - MessageTrailer.getSize();
        byte[] stringBuffer = new byte[stringSize];
        bBuffer.get( MessageHeader.getSize(), stringBuffer, 0, stringSize );

        message = new String( stringBuffer );

        System.out.println( "message = " + message );

        trailer = MessageTrailer.getTrailerFromBuffer( bBuffer,
                                                       MessageHeader.getSize() + stringSize );
        size = MessageHeader.getSize() + MessageTrailer.getSize() + message.length();
    }

    public int getSize()
    {
        return size;
    }

    public byte[] pack()
    {
        ByteBuffer packedBuffer = ByteBuffer.allocate( size + 1 );

        packedBuffer.put( header.pack() );
        packedBuffer.put( message.getBytes( Charset.forName( "US-ASCII") ) );
        packedBuffer.put( trailer.pack() );

        return packedBuffer.array();
    }

    public String getMessage()
    {
        return message;
    }
}
