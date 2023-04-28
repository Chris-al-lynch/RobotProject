
#include <getopt.h>

#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>

#include "Exceptions.h"
#include "Robot.h"
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
    args.port = -1;
    int opt;

    while( (opt=getopt( argc, argv, "p:a:" )) )
    {
        if( opt == -1 )
        {
            break;
        }
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
                break;
            }
            default:
            {
                cout << usage << endl << endl;
                cout << "argument " << opt << " is invalid" << endl;
                exit( EXIT_FAILURE );
            }
        }
    }

    if( (args.port == -1) || (args.address.empty()) )
    {
        cout << usage << endl << endl;
        cout << "Must specify both listen address and listen port" << endl;
        cout << "address = " << args.address << endl;
        cout << "port = " << args.port << endl;
        exit( EXIT_FAILURE );
    }

    return args;
}

int
main( int argc, char *argv[] )
{
    arguments args = parseArgs( argc, argv );

    try
    {
        Robot *robot = Robot::getInstance( args.address, args.port );

        Status status = STATUS_FULLY_OPERATIONAL;

        while( status != STATUS_FAILED )
        {
            status = robot->getStatus();
            sleep( 1 );
        }
    }
    catch( ServerException& se )
    {
        cout << se.what();

        if( se.getError() != -1 )
        {
            cout << ": " + string( strerror( se.getError() ) ) + "(" + to_string( se.getError() ) + ")";
        }

        cout << endl;
        exit( 1 );
    }
    catch( exception& e )
    {
        cout << "An unexpected error occurred: " + string( e.what() ) << endl;
        exit( 2 );
    }

    exit( 0 );
}
