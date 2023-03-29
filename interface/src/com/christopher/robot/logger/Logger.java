package com.christopher.robot.logger;

public class Logger
{
    private final LogLevel DEFAULT_LOG_LEVEL = LogLevel.LOGGER_INFO;
    private LogLevel level;

    private void log( LogLevel level, String message )
    {
        if( level.getLevel() <= this.level.getLevel() )
        {
            System.out.println( message );
        }
    }

    public Logger()
    {
        level = DEFAULT_LOG_LEVEL;
    }

    public Logger( LogLevel level )
    {
        this.level = level;
    }

    public void logError( String msg )
    {
        log( LogLevel.LOGGER_ERROR, "ERROR: " + msg );
    }

    public void logInfo( String msg )
    {
        log( LogLevel.LOGGER_INFO, msg );
    }

    public void logDebug( String msg )
    {
        log( LogLevel.LOGGER_DEBUG, "DEBUG: " + msg );
    }
}
