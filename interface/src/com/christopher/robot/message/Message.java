package com.christopher.robot.message;

import java.nio.ByteOrder;

public abstract class Message
{
    private static int messageId = 0;

    protected int getMessageId()
    {
        return ++messageId;
    }

    public static boolean isMessageValid( byte[] msgBuffer )
    {
        boolean valid = false;

        try
        {
            MessageHeader header = MessageHeader.getHeaderFromBuffer( msgBuffer,
                                                                      ByteOrder.LITTLE_ENDIAN );
            if( msgBuffer.length != header.getMsgLength() )
            {
                valid = false;
            }
            else
            {
                valid = true;
            }
        }
        catch( MessageException me )
        {
            valid = false;
        }

        return valid;
    }

    public static Message getMessageFromBuffer( byte[] buffer ) throws MessageException
    {
        return getMessageFromBuffer( buffer, ByteOrder.BIG_ENDIAN );
    }

    public static Message getMessageFromBuffer( byte[] buffer,
                                                ByteOrder order ) throws MessageException
    {
        MessageHeader header;

        try
        {
            header = MessageHeader.getHeaderFromBuffer( buffer, order );
        }
        catch( MessageException me )
        {
            throw new MessageException( "Message not valid: " + me, me );
        }

        Message message = null;

        switch( header.getMsgType() )
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
                message = new TestMessage( buffer, order );
                break;
            }
            case TEST_RESPONSE:
            {
                message = new TestResponse( buffer, order );
                break;
            }
            case INVALID_MESSAGE:
            default:
            {
                throw new MessageException( "Message Type was invalid: " + header.getMsgType() );
            }
        }

        return message;
    }

    public abstract int getSize();
    public abstract byte[] pack();
}
