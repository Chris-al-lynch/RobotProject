package com.christopher.robot.message;

import java.nio.ByteBuffer;

public abstract class Message
{
    protected MessageHeader header;
    protected MessageTrailer trailer;

    public abstract Response processMessage();
    public abstract void pack( ByteBuffer buffer );

    protected Message()
    {
    }

    protected Message( int dataLength, int type, int id, int sequenceId,
                       int version )
    {
        header = new MessageHeader( MessageHeader.getSize() + dataLength + MessageTrailer.getSize(),
                                    type, id, sequenceId, version );
        trailer = new MessageTrailer();
    }

    protected Message( int dataLength, int type, int id, int version )
    {
        header = new MessageHeader( MessageHeader.getSize() + dataLength + MessageTrailer.getSize(),
                                    type, id, version );
        trailer = new MessageTrailer();
    }

    public MessageHeader getHeader()
    {
        return header;
    }

    public MessageTrailer trailer()
    {
        return trailer;
    }

    public int getLength()
    {
        int length = 0;

        if( header != null )
        {
            length = header.getLength();
        }

        return length;
    }
}
