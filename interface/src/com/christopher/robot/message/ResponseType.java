package com.christopher.robot.message;

import java.util.HashMap;

public enum ResponseType
{
    INVALID_RESPONSE    (0),
    MOVEMENT_RESPONSE   (1),
    SENSE_RESPONSE      (2),
    TEST_RESPONSE       (3);

    private final int value;
    private static HashMap<Integer, ResponseType> map = new HashMap<>();

    private ResponseType( int value )
    {
        this.value = value;
    }

    static
    {
        for( ResponseType type : ResponseType.values() )
        {
            map.put( type.value, type );
        }
    }

    public static ResponseType getType( int value )
    {
        return map.get( value );
    }

    public int getValue()
    {
        return this.value;
    }
}
