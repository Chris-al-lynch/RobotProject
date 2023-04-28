#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <limits.h>
#include <pthread.h>

#include "Logger.h"
#include "Status.h"

using namespace std;

typedef struct motorSharedMemory
{
    sem_t registerChanged;
    pthread_mutex_t lock;
    bool stopMove;
    int moveRegister;
    int initComplete;
    bool shutdown;
} motorSharedMemory_t;

class Motor
{
    private:
        static const int FOREVER_MOVE_FORWARD  = INT_MAX;
        static const int FOREVER_MOVE_BACKWARD = INT_MIN;
        static const int SHARED_MEMORY_SIZE = sizeof( motorSharedMemory_t );
        Logger *logger;
        string name;
        bool stopMove;
        int sharedMemoryFD;
        motorSharedMemory_t *sharedMemoryPtr;
        Status status;
        pid_t childProcId;

        /* Runs in a separate process. */
        pthread_t motorThread;

        void initMotor();
        void initController();
        void move( int direction );
        void shutdown();
        static void *motorMotion( void *this_pointer );

    public:
        Motor( string name );
        ~Motor();

        Status getStatus();

        void moveForward();
        void moveForward( unsigned int amount );
        void moveBackward();
        void moveBackward( unsigned int amount );
        void stop();
};

#endif