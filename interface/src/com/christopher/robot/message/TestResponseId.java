package com.christopher.robot.message;

import java.util.HashMap;

public enum TestResponseId
{
    INVALID_RESONSE         (0),
    TEST_RESPONSE_SUCCESS   (1),
    TEST_RESPONSE_FAILURE   (2);

    private final int id;
    private static HashMap<Integer, TestResponseId> map = new HashMap<>();
    
    private TestResponseId( int id )
    {
        this.id = id;
    }

    static
    {
        for( TestResponseId id : TestResponseId.values() )
        {
            map.put( id.id, id );
        }
    }

    public static TestResponseId getId( int id )
    {
        return map.get( id );
    }

    public int getId()
    {
        return this.id;
    }
}
