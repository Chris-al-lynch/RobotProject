package com.christopher.robot.message;

import java.nio.ByteOrder;

public class TestResponse extends TestMessage
{

    public TestResponse( String message )
    {
        super( message );
        changeType();
    }

    public TestResponse( byte[] buffer, ByteOrder order ) throws MessageException
    {
        super( buffer, order );
    }

    public String getResponse()
    {
        return getMessage();
    }
}
