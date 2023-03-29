package com.christopher.robot.message;

import java.util.HashMap;

public enum MessageType
{
    INVALID_MESSAGE    (0),
    MOVEMENT_MESSAGE   (1),
    SENSE_MESSAGE      (2),
    TEST_MESSAGE       (3),
    TEST_RESPONSE      (4);

    private final int value;
    private static HashMap<Integer, MessageType> map = new HashMap<>();

    private MessageType( int value )
    {
        this.value = value;
    }

    static
    {
        for( MessageType type : MessageType.values() )
        {
            map.put( type.value, type );
        }
    }

    public static MessageType getType( int value )
    {
        return map.get( value );
    }

    public int getValue()
    {
        return this.value;
    }
}
