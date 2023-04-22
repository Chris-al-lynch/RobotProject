package com.christopher.robot.message;

import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

public class TestMessageFactory
{
    public static TestMessage generateTestMessageFromBuffer( TestMessageId id,
                                                             ByteBuffer buffer )
    {
        if( buffer != null )
        {
            String message = StandardCharsets.US_ASCII.decode( buffer ).toString();
            return generateTestMessage( id, message );
        }

        return null;
    }

    public static TestMessage generateTestMessage( TestMessageId id,
                                                   String message )
    {
        switch( id )
        {
            case TEST_MESSAGE_STRING:
            {
                return new TestMessage( id, message );
            }
            case INVALID_TEST_MESSAGE:
            default:
            {
                break;
            }
        }

        return null;
    }
}
