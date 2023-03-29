package com.christopher.robot.message;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class MessageHeader
{
    private int msgLength;
    private MessageType msgType;
    private int id;
    private int marker;
    private static final int HEADER_MARKER = 0x0BADDAD0;

    protected void chageType( MessageType newType )
    {
        msgType = newType;
    }

    public MessageHeader( int msgLength, MessageType msgType, int id )
    {
        this.msgLength = msgLength;
        this.msgType   = msgType;
        this.id        = id;
        this.marker    = HEADER_MARKER;
        System.out.println( "msgLength = " + msgLength );
    }

    public static MessageHeader getHeaderFromBuffer( byte[] buffer,
                                                     ByteOrder order ) throws MessageException
    {
        ByteBuffer wrapped = ByteBuffer.wrap( buffer.clone() );
        wrapped.order( order );
        return getHeaderFromBuffer( wrapped );
    }

    public static MessageHeader getHeaderFromBuffer( ByteBuffer buffer ) throws MessageException
    {
        int msgLength = buffer.getInt();
        int msgType   = buffer.getInt();
        int id        = buffer.getInt();
        int marker    = buffer.getInt();

        System.out.println( "msgLength = " + msgLength
                          + ", msgType = " + msgType
                          + ", id = " + id
                          + ", marker = " + String.format( "0x%X", marker ) );
        

        if( marker != HEADER_MARKER )
        {
            throw new MessageException( "Invalid Buffer...Marker invalid: " + marker );
        }

        MessageHeader header = new MessageHeader( msgLength, MessageType.getType( msgType ), id );

        return header;
    }


    public static int getSize()
    {
        return 4 * Integer.BYTES;
    }

    public byte[] pack()
    {
        ByteBuffer packedBuffer = ByteBuffer.allocate( 4 * Integer.BYTES );

        packedBuffer.putInt( msgLength );
        packedBuffer.putInt( msgType.getValue() );
        packedBuffer.putInt( id );
        packedBuffer.putInt( marker );

        return packedBuffer.array();
    }

    public int getMsgLength()
    {
        return msgLength;
    }

    public MessageType getMsgType()
    {
        return msgType;
    }

    public int getMsgId()
    {
        return id;
    }
}
