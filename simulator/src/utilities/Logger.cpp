
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <time.h>
#include <unistd.h>

#include "Logger.h"

using namespace std;

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
        ostringstream oss;
        oss << this_thread::get_id();
        pid_t pid = getpid();
        char timeStr[100];
        time_t currentTime = time( NULL );
        struct tm *p = localtime( &currentTime );
        strftime( timeStr, 100, "%Y/%m/%d %H:%M:%S", p );
        printf( "%s: pid %d tid %s - %s\n", timeStr, (int)pid,
                oss.str().c_str(), msg.c_str() );
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
