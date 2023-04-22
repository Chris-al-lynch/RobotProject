package com.christopher.robot.message;

import java.nio.ByteBuffer;

public class ResponseFactory
{
    public static Response createResponse( ByteBuffer buffer ) throws MessageException
    {
        MessageHeader header = MessageHeader.unpack( buffer );
        int dataLength = header.getLength() - MessageHeader.getSize()
                       - MessageTrailer.getSize();

        byte[] data;

        if( dataLength > 0 )
        {
            data = new byte[dataLength];
            buffer.get( data, 0, dataLength );
        }

        Response response = null;

        switch( ResponseType.getType( header.getType() ) )
        {
            case MOVEMENT_RESPONSE:
            {
                break;
            }
            case SENSE_RESPONSE:
            {
                break;
            }
            case TEST_RESPONSE:
            {
                response = TestMessageResponse.unpack( buffer );
                break;
            }
            case INVALID_RESPONSE:
            default:
            {
                break;
            }
        }

        return response;
    }
}
