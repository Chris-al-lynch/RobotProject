package com.christopher.robot.message;

import java.nio.ByteBuffer;

import com.christopher.robot.utilities.PrintUtilities;

public class MessageTrailer
{
    private int marker;
    private static final int TRAILER_MARKER = 0X0DADBAD0;

    public MessageTrailer()
    {
        marker = TRAILER_MARKER;
    }

    public static int getSize()
    {
        return Integer.BYTES;
    }

    public static MessageTrailer getTrailerFromBuffer( byte[] buffer, int location ) throws MessageException
    {
        ByteBuffer wrapper = ByteBuffer.wrap( buffer );
        return getTrailerFromBuffer( wrapper,  location ) ;
    }

    public static MessageTrailer getTrailerFromBuffer( ByteBuffer buffer, int location ) throws MessageException
    {
        int buffMarker = buffer.getInt( location );

        PrintUtilities.printByteArray( buffer.array() );

        System.out.println( "Trailer marker = " + String.format( "0x%08X", buffMarker ) );
        System.out.println( "Trailer marker 2 = " + buffMarker );

        if( buffMarker != TRAILER_MARKER )
        {
            throw new MessageException( "Invalid trailer marker in buffer." );
        }
       
        return new MessageTrailer();
    }

    public byte[] pack()
    {
        ByteBuffer packedBuffer = ByteBuffer.allocate( 4 );

        packedBuffer.putInt( marker );

        return packedBuffer.array();
    }
}
