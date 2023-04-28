
#include <cstring>

#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>
#include <wait.h>

#include "Exceptions.h"
#include "Motor.h"

Motor::Motor( string name )
{
    status = STATUS_INITIALIZING;
    try
    {
        logger = new Logger();
        this->name = name;

        /* Motors are going to be implemented as separate processes so I can
         * demonstrate cross-process communication and shared memory.
         */
        logger->logInfo( name + ") Initializing " + name + "..." );
        childProcId = fork();

        if( childProcId == -1 )
        {
            throw MotorException( name + ") Fork Error with motor." );
        }
        else if( childProcId > 0 )
        {
            /* Parent */
            logger->logInfo( name + ") Initializing the controller..." );
            initController();
        }
        else
        {
            /* child */
            logger->logInfo( name + ") Initializing the motor..." );
            initMotor();
            /* When you use fork to start your process, it creates a copy of
             * the current process.  That means if we return from here and
             * continue initializing the robot in two processes.  If we exit,
             * then the thread will exit.  We need to stop the process here. 
             */
            while( not sharedMemoryPtr->shutdown )
            {
                if( status == STATUS_FAILED )
                {
                    logger->logError( name + " motor failed." );
                }

                sleep( 1 );
            }

            exit( 0 );
        }
    }
    catch( exception& e )
    {
        status = STATUS_FAILED;
        throw e;
    }

    status = STATUS_FULLY_OPERATIONAL;
}

Motor::~Motor()
{
    shutdown();
}

void
Motor::shutdown()
{
    sharedMemoryPtr->shutdown = true;
    shm_unlink( name.c_str() );
    delete logger;
}

void
Motor::initController()
{
    logger->logInfo( name + ") Opening shared memory for creation for name = " + name + " ..." );
    /* Open a shared memory object. */
    
    sharedMemoryFD = shm_open( name.c_str(), O_CREAT | O_RDWR, 0660 );

    logger->logInfo( name + ") Setting memory size..." );
    /* Set the size of the shared memory. */
    ftruncate( sharedMemoryFD, SHARED_MEMORY_SIZE );

    logger->logInfo( name + ") Generating pointer to memory..." );
    /* Get a pointer to the memory. */
    sharedMemoryPtr = (motorSharedMemory_t *)mmap( NULL, SHARED_MEMORY_SIZE,
                                                   PROT_WRITE | PROT_READ,
                                                   MAP_SHARED, sharedMemoryFD, 0 );

    memset( sharedMemoryPtr, 0, sizeof( motorSharedMemory_t ) );


    /* Initialize the semaphore used to notify the motor that a register was
     * updated.
     */
    logger->logInfo( name + ") Initializing semaphore..." );
    if( sem_init( &sharedMemoryPtr->registerChanged, 0, 0 ) < 0 )
    {
        throw MotorException( "sem_init() failed with errno = "
                            + string( strerror( errno ) ) + "("
                            + to_string( errno ) + ")" );
    }

    /* Initialize the mutex. */
    logger->logInfo( name + ") Initializing mutex..." );
    if( pthread_mutex_init( &sharedMemoryPtr->lock, NULL ) < 0 )
    {
        throw MotorException( "pthread_mutex_init() failed with errno = "
                            + string( strerror( errno ) ) + "("
                            + to_string( errno ) + ")" );
    }

    logger->logInfo( name + ") Initializing memory..." );
    sharedMemoryPtr->stopMove = false;
    sharedMemoryPtr->moveRegister = 0;
    sharedMemoryPtr->initComplete = true;
    logger->logInfo( name + ") Controller init complete." );
}

void
Motor::initMotor()
{
    /* Give up the processor so that the parent will run first. */
    sleep( 1 );
    logger->logInfo( name + ") Motor opening shared memory for name = " + name + " ..." );
    /* Open a shared memory object. */
    sharedMemoryFD = shm_open( name.c_str(), O_RDWR, 0660 );

    /* Get a pointer to the shared memory. */
    logger->logInfo( name + ") Motor generating pointer to shared memory..." );
    sharedMemoryPtr = (motorSharedMemory_t *)mmap( NULL, SHARED_MEMORY_SIZE,
                                                   PROT_WRITE | PROT_READ,
                                                   MAP_SHARED, sharedMemoryFD, 0 );

    logger->logInfo( name + ") Motor waiting for controller init to complete." );
    do
    {
        sleep( 1 );
    } while( not sharedMemoryPtr->initComplete );

    logger->logInfo( name + ") Motor discovered controller init is complete." );

    /* pthread_create() expects a function passed in with the following
     * prototype: void *(*)(void *).  The prototype for a class method of
     * Motor will be void *(Motor::*)(void *).  So, I am not able to pass
     * a class method.  However, if you make that method a static method, then
     * the prototype is void *(*)(void *) which is what I'm looking for.
     * This means that I have to make motorMotion static to use as a thread
     * function.  So that it looks like a class method, I pass the this pointer
     * as a parameter into the function.  This way I can still have multiple
     * motors running this code, each with their own this pointer.
     */
    int rc = pthread_create( &motorThread, NULL, motorMotion, this );
    
    if( rc != 0 )
    {
        throw MotorException( name + ") pthread_create failed with error: "
                            + string( strerror( rc ) ) + "(" + to_string( rc )
                            + ")" );
    }
}

Status
Motor::getStatus()
{
    if( (childProcId != 0) && (status == STATUS_FULLY_OPERATIONAL) )
    {
        int wstatus = 0;

        pid_t rc = waitpid( childProcId, &wstatus, WNOHANG | WUNTRACED | WCONTINUED );

        if( rc < 0 )
        {
            logger->logError( name + " failed in waitpid() with "
                            + string( strerror( errno ) ) + "("
                            + to_string( errno ) + ")" );
        }
        else
        {
            /* If the child process has stopped for any reason, then we need
             * to fail.
             */
            if( WIFEXITED( wstatus ) || WIFSIGNALED( wstatus )
             || WCOREDUMP( wstatus ) || WIFSTOPPED( wstatus ) )
            {
                status = STATUS_FAILED;
            }
        }
    }

    return status;
}

void
Motor::moveForward()
{
    sharedMemoryPtr->moveRegister = FOREVER_MOVE_FORWARD;
    sem_post( &sharedMemoryPtr->registerChanged );
}

void
Motor::moveForward( unsigned int amount )
{
    sharedMemoryPtr->moveRegister = amount;
    sem_post( &sharedMemoryPtr->registerChanged );
}

void
Motor::moveBackward()
{
    sharedMemoryPtr->moveRegister = FOREVER_MOVE_BACKWARD;
    sem_post( &sharedMemoryPtr->registerChanged );
}

void
Motor::moveBackward( unsigned int amount )
{
    int directionAmount = amount;
    if( directionAmount < 0 )
    {
        directionAmount = amount * -1;
    }

    sharedMemoryPtr->moveRegister = directionAmount;
    sem_post( &sharedMemoryPtr->registerChanged );
}

void
Motor::stop()
{
    sharedMemoryPtr->moveRegister = 0;
    sem_post( &sharedMemoryPtr->registerChanged );
}

void
Motor::move( int direction )
{
    if( direction > 0 )
    {
        logger->logInfo( name + ") Moving forward" );
    }
    else if( direction < 0 )
    {
        logger->logInfo( name + ") Moving backward" );
    }
}

void *
Motor::motorMotion( void *thisPointer )
{
    Motor *this_p = (Motor *)thisPointer;
    try
    {
        this_p->logger->logInfo( this_p->name + ") Motor in motion..." );
        while( true )
        {
            this_p->logger->logInfo( this_p->name + ") Motor waiting for register change..." );
            if( sem_wait( &this_p->sharedMemoryPtr->registerChanged ) < 0 )
            {
                throw MotorException( "pthread_mutex_lock() failed with errno = "
                                    + string( strerror( errno ) ) + "("
                                    + to_string( errno ) + ")" );
            }

            this_p->logger->logInfo( this_p->name + ") Motor Reading register" );
            int moveAmount = this_p->sharedMemoryPtr->moveRegister;
            int savedMoveAmount = moveAmount;

            this_p->logger->logInfo( this_p->name + ") Motor checking direction" );
            int direction = 1;
            if( moveAmount < 0 )
            {
                direction = -1;
                moveAmount = moveAmount * -1;
            }

            this_p->logger->logInfo( this_p->name + ") Motor getting ready to move" );
            for( int m = 0; m < moveAmount; m++ )
            {
                if( this_p->stopMove || (this_p->sharedMemoryPtr->moveRegister != savedMoveAmount) )
                {
                    break;
                }

                this_p->logger->logInfo( ") Motor Moving..." );
                this_p->move( direction );

                sleep( 1 );
            }
        }
    }
    catch( exception& e )
    {
        this_p->status = STATUS_FAILED;
        this_p->logger->logError( "Motor " + this_p->name + " failed with: " + string( e.what() ) );
        this_p->shutdown();
        throw e;
    }
}
