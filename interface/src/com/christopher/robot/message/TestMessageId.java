package com.christopher.robot.message;

import java.util.HashMap;

public enum TestMessageId
{
    INVALID_TEST_MESSAGE   (0),
    TEST_MESSAGE_STRING    (1);

    private final int id;
    private static HashMap<Integer, TestMessageId> map = new HashMap<>();
    
    private TestMessageId( int id )
    {
        this.id = id;
    }

    static
    {
        for( TestMessageId id : TestMessageId.values() )
        {
            map.put( id.id, id );
        }
    }

    public static TestMessageId getId( int id )
    {
        return map.get( id );
    }

    public int getId()
    {
        return this.id;
    }
}
