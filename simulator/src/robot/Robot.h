#ifndef __ROBOT_H__
#define __ROBOT_H__

#include <mutex>
#include <string>
#include <unordered_map>

#include "Brain.h"
#include "Logger.h"
#include "Motor.h"
#include "Status.h"

using namespace std;

class Robot
{
    private:
       Logger *logger;
       Brain<Robot> *brain;
       Motor *leftMotor;
       Motor *rightMotor;
       string address;
       int port;
       Status robotStatus; 

       static unordered_map <string, unordered_map<int, Robot *>> instances;
       static mutex instanceLock;

       static const int MAX_RECOVERIES = 3;
       int recoveryCount;
       static const long MAX_RECOVERY_TIME = 60L;
       long recoveryTimestamp;

       jthread *monitorThread;

       Robot( string address, int port );
       void monitor();
       void resetRobot();
       void start();
       void shutdown();

    public:
       static Robot *getInstance( string address, int port );

       Status getStatus();

       void moveForward();
       void moveForwardAmount( int amount );
       void moveBackward();
       void moveBackwardAmount( int amount );
       void moveLeft();
       void moveLeftAmount( int amount );
       void moveRight();
       void moveRightAmount( int amount );
       void stop();
};

#endif
