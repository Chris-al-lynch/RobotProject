package com.christopher.robot;

import com.christopher.robot.client.Client;
import com.christopher.robot.message.Message;
import com.christopher.robot.message.TestMessage;
import com.christopher.robot.message.TestResponse;

/**
 */
class RobotInterface
{
    private static String address = null;
    private static int    port = -1;
    private static String testMessage = null;
    private static String usage = "usage: java -jar dist/lib/RobotInterface.jar -a <server address> -p <server port> -m <Message to send>";

    private static void parseArgs( String[] args )
    {
        if( (args == null) || (args.length == 0) )
        {
            System.out.println( "ERROR: No parameters sepcified." );
            System.out.println( usage );
            System.exit( 1 );
        }
        for( int arg = 0; arg < args.length; arg++ )
        {
            switch( args[arg] )
            {
                case "-a":
                {
                    address = args[++arg];
                    break;
                }
                case "-p":
                {
                    port = Integer.parseInt( args[++arg] );
                    break;
                }
                case "-m":
                {
                    testMessage = args[++arg];
                    break;
                }
                default:
                {
                    System.out.println( "Error: Invalid parameter - " + args[arg] );
                    System.out.println( usage );
                    System.exit( 1 );
                }
            }
        }

        boolean error = false;

        if( address == null )
        {
            System.out.println( "Must specify server address." );
            error = true;
        }

        if( port == -1 )
        {
            System.out.println( "Must specify server port." );
            error = true;
        }

        if( testMessage == null )
        {
            System.out.println( "Must specify test message." );
            error = true;
        }

        if( error )
        {
            System.out.println( usage );
            System.exit( 1 );
        }

    }

    /**
     * Just a Tester for now.
     */
    public static void main( String[] args )
    {
        System.out.println( "Hello world, from RobotInterface!!!" );
        parseArgs( args );

        try
        {
            Client client = new Client( address, port );
            Message response = client.sendMessage( new TestMessage( testMessage ) );
            String responseMessage = ((TestResponse)response).getResponse();
            System.out.println( "Server returned: " + responseMessage );
        }
        catch( Exception e )
        {
            System.out.println( "A failure occurred: " + e );
            e.printStackTrace();
            System.exit( 1 );
        }

        System.exit( 0 );
    }
}