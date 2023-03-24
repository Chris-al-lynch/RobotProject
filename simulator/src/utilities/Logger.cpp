
#include <iostream>
#include <string>

#include "Logger.h"

Logger::Logger() : level( DEFAULT_LOG_LEVEL )
{
}

Logger::Logger( logLevel level ) : level( level )
{
}

void
Logger::log( logLevel level, string msg )
{
    if( level <= this->level )
    {
        cout << msg << endl;
    }
}

void
Logger::logError( string msg )
{
    log( LOGGER_ERROR, "ERROR: " + msg );
}

void
Logger::logInfo( string msg )
{
    log( LOGGER_INFO, msg );
}

void
Logger::logDebug( string msg )
{
    log( LOGGER_DEBUG, "DEBUG: " + msg );
}
