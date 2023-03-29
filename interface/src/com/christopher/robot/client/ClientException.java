package com.christopher.robot.client;

public class ClientException extends Exception
{
    public ClientException()
    {
        super();
    }

    public ClientException( String message )
    {
        super( message );
    }

    public ClientException( String message, Throwable cause )
    {
        super( message, cause );
    }
}
