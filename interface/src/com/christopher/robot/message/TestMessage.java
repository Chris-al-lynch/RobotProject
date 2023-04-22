package com.christopher.robot.message;

import java.nio.ByteBuffer;
import java.nio.charset.Charset;

public class TestMessage extends Message
{
    private static int TEST_MESSAGE_VERSION = 0;
    private String message;

    private TestMessage()
    {
        super();
    }

    public TestMessage( TestMessageId id, String message )
    {
        super( message.length(), MessageType.TEST_MESSAGE.getValue(),
               id.getId(), TEST_MESSAGE_VERSION );
        this.message = message;
    }

    public String getMessage()
    {
        return message;
    }

    private void setMessage( String message )
    {
        this.message = message;
    }

    public Response processMessage()
    {
        System.out.println( "Received: " + message );
        return new TestMessageResponse( TestResponseId.TEST_RESPONSE_SUCCESS,
                                        "Received message: " + message );
    }

    public void pack( ByteBuffer buffer )
    {
        header.pack( buffer );
        buffer.put( message.getBytes( Charset.forName( "US-ASCII") ) );
        trailer.pack( buffer );
    }

    public static TestMessage unpack( ByteBuffer buffer ) throws MessageException
    {
        TestMessage tm = new TestMessage();

        tm.header = MessageHeader.unpack( buffer );
        tm.trailer = MessageTrailer.unpack( buffer );

        int dataLength = tm.header.getLength() - MessageHeader.getSize()
                       - MessageTrailer.getSize();
        byte[] data = new byte[dataLength];
        buffer.get( data, MessageHeader.getSize(), dataLength );

        tm.setMessage( new String( data, Charset.forName( "US-ASCII" ) ) );

        return tm;
    }
}
