package com.christopher.robot.utilities;

public class PrintUtilities
{
    public static void printByteArray( byte[] array )
    {
        for( int b = 0; b < array.length; b++ )
        {
            System.out.print( String.format( "%02X", array[b] ) + " " );
            if( (b != 0) && ((b % 8) == 0) )
            {
                System.out.print( "\n" );
            }
        }

        System.out.print( "\n" );
    }
}
