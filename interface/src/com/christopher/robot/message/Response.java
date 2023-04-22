package com.christopher.robot.message;

import java.nio.ByteBuffer;

public abstract class Response
{
    protected MessageHeader header;
    protected MessageTrailer trailer;

    public abstract void pack( ByteBuffer buffer ); 
    //public abstract Response unpack( ByteBuffer buffer );

    protected Response()
    {
    }

    protected Response( int dataLength, int type, int id, int sequenceId,
                        int version )
    {
        header = new MessageHeader( MessageHeader.getSize() + dataLength + MessageTrailer.getSize(),
                                    type, id, sequenceId, version );
        trailer = new MessageTrailer();
    }

    protected Response( int dataLength, int type, int id, int version )
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
}
