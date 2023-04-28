#ifndef __BRAIN_H__
#define __BRAIN_H__

#include <cstring>
#include <string>

#include "Logger.h"
#include "Server.h"
#include "Status.h"

template<class WhosBrain>
class Brain
{
    private:
       enum movement
       {
           MOVE_FORWARD,
           MOVE_BACKWARD,
           MOVE_LEFT,
           MOVE_RIGHT,
           STOP,
           NUMBER_OF_MOVEMENTS
       };

       Logger *logger;

       void (WhosBrain::*MovementTable[NUMBER_OF_MOVEMENTS])();
       void (WhosBrain::*MovementAmountTable[NUMBER_OF_MOVEMENTS])( int );

       WhosBrain *owner;
       Server *server;
       MessageProcessor *messageProcessor;
       Status status;

    public:
        Brain( WhosBrain *owner, string address, int port )
        {
            status = STATUS_INITIALIZING;

            try
            {
                logger = new Logger();

                this->owner = owner;

                logger->logInfo( "Initializing memory..." );
                memset( MovementTable, 0, sizeof( void (*)() ) * NUMBER_OF_MOVEMENTS );
                memset( MovementAmountTable, 0,
                        sizeof( void (*)() ) * NUMBER_OF_MOVEMENTS );

                logger->logInfo( "Initializing the communications processor..." );
                messageProcessor = new MessageProcessor();

                logger->logInfo( "Initializing the communications interface..." );
                server = new Server( address, port );
                logger->logInfo( "Brain Initialization complete..." );
            }
            catch( exception& e )
            {
                status = STATUS_FAILED;
                logger->logError( "Brain constructor failed with: " + string( e.what() ) );
                throw e;
            }

            status = STATUS_FULLY_OPERATIONAL;
        }

        ~Brain()
        {
            delete server;
        }

        Status getStatus()
        {
            if( status == STATUS_FULLY_OPERATIONAL )
            {
                status = server->getStatus();

                if( status == STATUS_FULLY_OPERATIONAL )
                {
                    status = messageProcessor->getStatus();
                }
            }

            return status;
        }

        void registerMoveForwardFunction( void (WhosBrain::*moveForward)() )
        {
            MovementTable[MOVE_FORWARD] = moveForward;
        }

        void registerMoveForwardAmountFunction( void (WhosBrain::*moveForwardAmount)( int ) )
        {
            MovementAmountTable[MOVE_FORWARD] = moveForwardAmount;
        }

        void registerMoveBackwardFunction( void (WhosBrain::*moveBackward)() )
        {
            MovementTable[MOVE_BACKWARD] = moveBackward;
        }

        void registerMoveBackwardAmountFunction( void (WhosBrain::*moveBackwardAmount)( int ) )
        {
            MovementAmountTable[MOVE_BACKWARD] = moveBackwardAmount;
        }

        void registerMoveLeftFunction( void (WhosBrain::*moveLeft)() )
        {
            MovementTable[MOVE_LEFT] = moveLeft;
        }

        void registerMoveLeftAmountFunction( void (WhosBrain::*moveLeftAmount)( int ) )
        {
            MovementAmountTable[MOVE_LEFT] = moveLeftAmount;
        }

        void registerMoveRightFunction( void (WhosBrain::*moveRight)() )
        {
            MovementTable[MOVE_RIGHT] = moveRight;
        }

        void registerMoveRightAmountFunction( void (WhosBrain::*moveRightAmount)( int ) )
        {
            MovementAmountTable[MOVE_RIGHT] = moveRightAmount;
        }

        void registerStopFunction( void (WhosBrain::*stop)() )
        {
            MovementTable[STOP] = stop;
        }
};

#endif
