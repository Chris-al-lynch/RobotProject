#ifndef _CLIENT_EXCEPTION_H_
#define _CLIENT_EXCEPTION_H_

#include <cstring>
#include <exception>
#include <iostream>
#include <string>

using namespace std;

class ClientException : public exception
{
    private:
       int error;

    protected:
        string message;

    public:
       ClientException( const string& msg, int error = -1 ) : error( error ), message( msg ) {};
       ClientException( const char *msg, int error = -1 ) : error( error ), message( msg ) {};

       int getError()
       {
           return error;
       }

       const char *what() const throw ()
       {
           return message.c_str();
       }
};

#endif

