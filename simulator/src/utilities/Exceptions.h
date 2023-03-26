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
       ServerException( const string& msg, int error = -1) : error( error ), message( msg ) {};
       ServerException( const char *msg, int error = -1 ) : error( error ), message( msg ) {};

       int getError()
       {
           return error;
       }
       const char *what() const throw ()
       {
           return message.c_str();
       }
};

class LostConnectionException : public ServerException
{
    public:
        LostConnectionException( const string& msg ):ServerException( msg ) {};
        LostConnectionException( const char *msg ):ServerException( msg ) {};

        const char *what() const throw()
        {
            return message.c_str();
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

       int getId()
       {
          return id;
       }

       const char *what() const throw ()
       {
           return message.c_str();
       }
};

#endif