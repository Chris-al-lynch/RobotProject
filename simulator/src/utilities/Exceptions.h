#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include <cstring>
#include <exception>
#include <string>

#include <iostream>
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

class TcpException : public exception
{
    private:
       int error = -1;

    protected:
        string message;

    public:
       TcpException( const string& msg, int error = -1) : error( error ), message( msg ) {};
       TcpException( const char *msg, int error = -1 ) : error( error ), message( msg ) {};

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

class InvalidParameterException : public exception
{
    private:
        string message;

    public:
       InvalidParameterException( const string& msg ) : message( msg ) { cout << "In the exception" << endl; }
       InvalidParameterException( const char *msg ) : message( msg ) { cout << "In the exception" << endl; }

       const char *what() const throw ()
       {
           return message.c_str();
       }
};

class BufferOverflowException : public exception
{
    private:
        string message;

    public:
       BufferOverflowException( const string& msg ) : message( msg ) {};
       BufferOverflowException( const char *msg ) : message( msg ) {};

       const char *what() const throw ()
       {
           return message.c_str();
       }
};

class MotorException : public exception
{
    private:
        string message;

    public:
       MotorException( const string& msg ) : message( msg ) {};
       MotorException( const char *msg ) : message( msg ) {};

       const char *what() const throw ()
       {
           return message.c_str();
       }
};

#endif