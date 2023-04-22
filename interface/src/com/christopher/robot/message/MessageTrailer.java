package com.christopher.robot.message;

import java.nio.ByteBuffer;

public class MessageTrailer
{
    private int marker;
    private static final int TRAILER_MARKER = 0XDAD00BAD;

    public MessageTrailer()
    {
        marker = TRAILER_MARKER;
    }

    public static int getSize()
    {
        return Integer.BYTES;
    }

    public static MessageTrailer unpack( ByteBuffer buffer ) throws MessageException
    {
        int trailerIndex  = buffer.getInt( 0 ) - Integer.BYTES;
        int trailerMarker = buffer.getInt( trailerIndex );

        if( trailerMarker != TRAILER_MARKER )
        {
            throw new MessageException( "Buffer does not contain a valid trailer" );
        }

        return new MessageTrailer();
    }

    public void pack( ByteBuffer buffer )
    {
        buffer.putInt( marker );
    }
}
