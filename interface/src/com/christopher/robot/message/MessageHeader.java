package com.christopher.robot.message;

import java.nio.ByteBuffer;

public class MessageHeader
{
    private static int NewSequenceID;
    private int length;
    private int type;
    private int id;
    private int sequenceId;
    private int version;
    private int marker;
    private static final int HEADER_MARKER = 0xBAD00DAD;

    public MessageHeader( int length, int type, int id,
                           int version )
    {
        this( length, type, id, ++NewSequenceID, version );
    }

    public MessageHeader( int length, int type, int id, int sequenceId,
                           int version )
    {
        this.length     = length;
        this.type       = type;
        this.id         = id;
        this.sequenceId = sequenceId;
        this.version    = version;
        this.marker     = HEADER_MARKER;
    }

    public static int getSize()
    {
        return 6 * Integer.BYTES;
    }

    public int getLength()
    {
        return length;
    }

    public int getType()
    {
        return type;
    }

    public int getId()
    {
        return id;
    }

    public int getSequenceId()
    {
        return sequenceId;
    }

    public int getVersion()
    {
        return version;
    }

    public void pack( ByteBuffer buffer )
    {
        buffer.putInt( length );
        buffer.putInt( type );
        buffer.putInt( id );
        buffer.putInt( sequenceId );
        buffer.putInt( version );
        buffer.putInt( marker );
    }

    public static MessageHeader unpack( ByteBuffer buffer )
                                                         throws MessageException
    {
        int currentPosition = buffer.position();
        buffer.rewind();

        int length  = buffer.getInt();
        int type    = buffer.getInt();
        int id      = buffer.getInt();
        int sequenceId = buffer.getInt();
        int version = buffer.getInt();
        int marker  = buffer.getInt();

        buffer.position( currentPosition );

        if( marker != HEADER_MARKER )
        {
            throw new MessageException( "Invalid Buffer...Marker invalid: " + marker );
        }

        return new MessageHeader( length, type, id, sequenceId, version );
    }
}
