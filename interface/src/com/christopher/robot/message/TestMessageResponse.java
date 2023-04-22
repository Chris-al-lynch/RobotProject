package com.christopher.robot.message;

import java.nio.ByteBuffer;
import java.nio.charset.Charset;

public class TestMessageResponse extends Response
{
    private String responseMessage;
    private static final int TEST_RESPONSE_VERSION = 0;

    private TestMessageResponse()
    {
        super();
    }

    public TestMessageResponse( TestResponseId id, String responseMessage )
    {
        super( responseMessage.length(), ResponseType.TEST_RESPONSE.getValue(),
               id.getId(), TEST_RESPONSE_VERSION );
        this.responseMessage = responseMessage;
    }

    public String getMessage()
    {
        return responseMessage;
    }

    private void setMessage( String message )
    {
        responseMessage = message;
    }

    public void pack( ByteBuffer buffer )
    {
        header.pack( buffer );
        buffer.put( responseMessage.getBytes( Charset.forName( "US-ASCII" ) ) );
        trailer.pack( buffer );
    }

    public static TestMessageResponse unpack( ByteBuffer buffer )
                                                         throws MessageException
    {
        TestMessageResponse response = new TestMessageResponse();
        
        response.header = MessageHeader.unpack( buffer );
        response.trailer = MessageTrailer.unpack( buffer );
        int dataLength = response.header.getLength() - MessageHeader.getSize()
                       - MessageTrailer.getSize();
        byte[] data = new byte[dataLength];

        buffer.position( MessageHeader.getSize() );
        buffer.get( data, 0, dataLength );
        response.setMessage( new String( data, Charset.forName( "US-ASCII" ) ) );

        return response;
    }
}
