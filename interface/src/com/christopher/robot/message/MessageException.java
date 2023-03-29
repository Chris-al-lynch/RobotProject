package com.christopher.robot.message;

public class MessageException extends Exception
{
    public MessageException()
    {
        super();
    }

    public MessageException( String message )
    {
        super( message );
    }

    public MessageException( String message, Throwable cause )
    {
        super( message, cause );
    }
}