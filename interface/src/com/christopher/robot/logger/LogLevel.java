package com.christopher.robot.logger;

public enum LogLevel
{
    LOGGER_ERROR (0),
    LOGGER_INFO  (1),
    LOGGER_DEBUG (2);

    private final int level;
    LogLevel( int level )
    {
        this.level = level;
    }

    int getLevel()
    {
        return this.level;
    }
}
