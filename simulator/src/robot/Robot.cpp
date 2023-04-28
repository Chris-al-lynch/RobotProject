
#include <chrono>

#include "Status.h"
#include "Robot.h"

unordered_map<string, unordered_map<int, Robot *>> Robot::instances;
mutex Robot::instanceLock;

Robot::Robot( string address, int port )
{
    robotStatus = STATUS_INITIALIZING;
    logger = new Logger();

    recoveryCount     = 0;
    recoveryTimestamp = 0L;
    this->address = address;
    this->port = port;

    start();

    logger->logInfo( "Adding instructions..." );
    brain->registerMoveForwardFunction( &Robot::moveForward );
    brain->registerMoveBackwardFunction( &Robot::moveBackward );
    brain->registerMoveRightFunction( &Robot::moveForward );
    brain->registerMoveLeftFunction( &Robot::moveBackward );

    brain->registerMoveForwardAmountFunction( &Robot::moveForwardAmount );
    brain->registerMoveBackwardAmountFunction( &Robot::moveBackwardAmount );
    brain->registerMoveRightAmountFunction( &Robot::moveForwardAmount );
    brain->registerMoveLeftAmountFunction( &Robot::moveBackwardAmount );

    logger->logInfo( "Wakeup complete." );

    monitorThread = new jthread( &Robot::monitor, this );
    robotStatus = STATUS_FULLY_OPERATIONAL;
}

void
Robot::start()
{
    logger->logInfo( "Waking the brain..." );
    brain = new Brain<Robot>( this, address, port );

    logger->logInfo( "Waking the left leg..." );
    leftMotor = new Motor( "leftMotor" );

    logger->logInfo( "Waking the right leg..." );
    rightMotor = new Motor( "rightMotor" );
}

Robot *
Robot::getInstance( string address, int port )
{
    Robot *robot = nullptr;

    instanceLock.lock();
    {
        if( instances.find( address ) != instances.end() )
        {
            if( instances[address].find( port ) != instances[address].end() )
            {
                robot = instances[address][port];
            }
        }

        if( robot == nullptr )
        {
            robot = new Robot( address, port );

            instances[address][port] = robot;
        }
    }
    instanceLock.unlock();

    return robot;
}

void
Robot::monitor()
{
    while( true )
    {
        Status status = brain->getStatus();

        if( status == STATUS_FULLY_OPERATIONAL )
        {
            status = leftMotor->getStatus();

            if( status == STATUS_FULLY_OPERATIONAL )
            {
                status = rightMotor->getStatus();
            }
        }

        switch( status )
        {
            case STATUS_FULLY_OPERATIONAL:
            {
                robotStatus = STATUS_FULLY_OPERATIONAL;

                if( (recoveryCount > 0) || (recoveryTimestamp > 0L) )
                {
                    long currentTime = duration_cast<chrono::seconds>(chrono::system_clock::now().time_since_epoch()).count();

                    if( (currentTime - recoveryTimestamp) > MAX_RECOVERY_TIME )
                    {
                        recoveryCount = 0;
                        recoveryTimestamp = 0;
                    }
                }

                break;
            }
            case STATUS_FAILED:
            {
                robotStatus = STATUS_RECOVERY_IN_PROGRESS;

                logger->logError( "A failure occurred.  The robot is no longer fully operational." );
                resetRobot();
                recoveryCount++;

                if( recoveryTimestamp == 0 )
                {
                    recoveryTimestamp = duration_cast<chrono::seconds>(chrono::system_clock::now().time_since_epoch()).count();
                }

                long currentTime = duration_cast<chrono::seconds>(chrono::system_clock::now().time_since_epoch()).count();

                if( recoveryCount > MAX_RECOVERIES )
                {
                    robotStatus = STATUS_FAILED;
                    logger->logError( "Robot recovery failed.  Giving up." );

                    break;
                }

                break;
            }
        }

        sleep( 1 );
    }
}

void
Robot::resetRobot()
{
    shutdown();
    start();
}

void
Robot::shutdown()
{
    delete leftMotor;
    delete rightMotor;
    delete brain;
}

void
Robot::moveForward()
{
    leftMotor->moveForward();
    rightMotor->moveForward();
}

void
Robot::moveForwardAmount( int amount )
{
    leftMotor->moveForward( amount );
    rightMotor->moveForward( amount );
}
void
Robot::moveBackward()
{
    leftMotor->moveBackward();
    rightMotor->moveBackward();
}
void
Robot::moveBackwardAmount( int amount )
{
    leftMotor->moveBackward( amount );
    rightMotor->moveBackward( amount );
}
void
Robot::moveLeft()
{
    rightMotor->moveForward();
}
void
Robot::moveLeftAmount( int amount )
{
    rightMotor->moveForward( amount );
}
void
Robot::moveRight()
{
    leftMotor->moveForward();
}
void
Robot::moveRightAmount( int amount )
{
    leftMotor->moveForward( amount );
}

void
Robot::stop()
{
    rightMotor->stop();
    leftMotor->stop();
}

Status
Robot::getStatus()
{
    return robotStatus;
}