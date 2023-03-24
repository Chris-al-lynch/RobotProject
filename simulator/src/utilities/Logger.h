#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>

using namespace std;

typedef enum logLevel
{
    LOGGER_ERROR,
    LOGGER_INFO,
    LOGGER_DEBUG
} logLevel;
class Logger
{
    private:
        const logLevel DEFAULT_LOG_LEVEL = LOGGER_INFO;
        logLevel level;

        void log( logLevel level, string msg );

    public:
        Logger();
        Logger( logLevel level );

        void logError( string msg );
        void logInfo( string msg );
        void logDebug( string msg );
};

#endif