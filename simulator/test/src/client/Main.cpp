
#include <getopt.h>

#include <iostream>
#include <string>

#include "Client.h"
#include "ClientException.h"

using namespace std;

typedef struct arguments
{
    int port;
    string address;
    string testMessage;
} arguments;

arguments
parseArgs( int argc, char *argv[] )
{
    string usage = "server -a <address> -p <port number> -m <test message>";
    arguments args;
    int opt;

    while( (opt=getopt( argc, argv, "p:a:m:" )) )
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
            case 'm':
            {
                args.testMessage = optarg;
                break;
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

    try
    {
        Client client( args.address, args.port );

        cout << "Calling sendTestMessage( " + args.testMessage + ")" << endl;
        client.sendTestMessage( args.testMessage );
    }
    catch( ClientException *ce )
    {
        cout << "Send Test Message Failed: " << endl;
        cout << string( ce->what() ) << endl;
        delete ce;
    }
    catch( const exception& e )
    {
        cout << "Unexpected Send Test Message Failed: " << endl;
        printf( "%s\n", e.what() );
        //cout << string( e.what() ) << endl;
    }
    
    exit( 0 );
}
