#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include <cstring>
#include <exception>
#include <string>

using namespace std;

class ServerException : public exception
{
    private:
       int error = -1;

    protected:
        string message;

    public:
       ServerException( const string& msg ) : message( msg ) {};
       ServerException( const char *msg ) : message( msg ) {};
       ServerException( int error, const string& msg ) : error( error ), message( msg ) {};
       ServerException( int error, const char *msg ) : error( error ), message( msg ) {};

       const char *what() const throw ()
       {
           string errorMsg = "An unexpected server error occurred: " + message;

           if( error > 0 )
           {
               errorMsg += (" - " + string( strerror( error ) ) + " (" + to_string( error ) + ")" );
           }

           return( errorMsg.c_str() );
       }
};

class LostConnectionException : public ServerException
{
    public:
        LostConnectionException( const string& msg ):ServerException( msg ) {};
        LostConnectionException( const char *msg ):ServerException( msg ) {};

        const char *what() const throw()
        {
            return( ("A Lost Connection error occurred: " + message).c_str() );
        }
};

class MessageException : public exception
{
    private:
        string message;
        int id;

    public:
       MessageException( const string& msg, int id = -1 ) : message( msg ), id( id ) {};
       MessageException( const char *msg, int id = -1 ) : message( msg ), id( id ) {};

       const char *what() const throw ()
       {
           string errorMsg = "An unexpected Message error occurred";
           
           if( id != -1 )
           {
               errorMsg += (" while processing message id " + to_string( id ) );
           }

           errorMsg += (": " + message);

           return( errorMsg.c_str() );
       }
};

#endif