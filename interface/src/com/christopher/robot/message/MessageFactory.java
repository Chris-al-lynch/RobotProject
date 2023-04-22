package com.christopher.robot.message;

import java.nio.ByteBuffer;

public class MessageFactory
{
    public static Message createMessage( ByteBuffer buffer ) throws MessageException
    {
        MessageHeader header   = MessageHeader.unpack( buffer );
        int dataLength         = header.getLength() - MessageHeader.getSize()
                               - MessageTrailer.getSize();
        int offset = MessageHeader.getSize();

        byte[] data = new byte[dataLength];

        buffer.get( data, offset, dataLength );

        Message message = null;

        switch( MessageType.getType( header.getType() ) )
        {
            case MOVEMENT_MESSAGE:
            {
                break;
            }
            case SENSE_MESSAGE:
            {
                break;
            }
            case TEST_MESSAGE:
            {
                message = TestMessage.unpack( buffer );
                break;
            }
            case INVALID_MESSAGE:
            default:
            {
                break;
            }
        }

        return message;
    }


}
