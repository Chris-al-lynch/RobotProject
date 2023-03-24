
#include <getopt.h>
#include <iostream>
#include <string>

#include "Server.h"

using namespace std;

typedef struct arguments
{
    int port;
    string address;
} arguments;

arguments
parseArgs( int argc, char *argv[] )
{
    string usage = "server -a <address> -p <port number>";
    arguments args;
    int opt;

    while( (opt=getopt( argc, argv, "p:a:" )) )
    {
        switch( opt )
        {
            case 'a':
            {
                args.address = optarg;
                break;
            }
            case 'p':
            {
                args.port = atoi( optarg );
            }
            default:
            {
                cout << usage << endl << endl;
                exit( EXIT_FAILURE );
            }
        }
    }

    return args;
}


int
main( int argc, char *argv[] )
{
    arguments args = parseArgs( argc, argv );

    Server server( args.address, args.port );

    server.start();

    exit( 0 );
}