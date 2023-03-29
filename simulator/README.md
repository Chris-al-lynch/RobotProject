C++ concepts found here: classes, Inheritance, Receiving messages and sending responses through a TCP socket as a server, Queues, mutexes, counting_semaphores, jthreads and using class methods as jthread functions.

NOTE: There is a lot of cleanup that still needs to be done as well as commenting.  I will get to this as soon as possible.
NOTE: I made some changes converting from big endian to little endian due to issues in the java code (see __builtin_bswap32()).  This will be removed when I fix the Java code.

---------------------------------------------------------------------------------------------------------

This component is a simulator for a robot.  I don't have a real robot at the moment, so I am going to simulate a robot with software.  I would eventually like to hook up a webcam and simulate the robot having a camera and stream it back to the client/controller.

Progress:

This component was going to be one of the last ones I worked on.  However, I have a coding interview in C++ coming up and need some C++ practice.  So, I'm going to do some work on it now.

I have written a TCP server which receives messages from a client, processes the message, and returns a response back to the sender.  I wrote a test client that sends a test message receives the response and validates that the response is correct.  However, this test client is currently broken since I made some change for the Java code.  I will remove those changes soon which will allow this test client to work again.  I will describe each of the classes used to develop this code.  I still need to go through and comment the code.  However, that will have to be done later.  I have a Java coding interview coming up soon, and I need to practice Java.  It's been awhile.

utilities:

Exceptions.cpp contains multiple exception classes.  I like to create my own exceptions so it is easier to see which component failed.

Logger - I always create a Logger class for most big projects.  It makes it easier to change how things are logged.  Right now, everything is going to standard out.  If later I want to send things to a file or both to a file and standard out, I only have to change it in the Logger class.

utils.[h|cpp] - General utilities will go here.  Currently there is a function to convert an integer/short/long to an hex string.


server:

The Server class implements a TCP server.  The address and port on which the server listens are configurable at start time.  It would be interesting to make this configurable at run time.  Maybe I can add this enhancement in the future.  I havent finished the stop code yet because I am killing the process to kill the server.  I will have a clean shutdown by the time I'm finished.  The server waits for a message on the configured port.  Once one is received it loads the message in a buffer and throws it on a queue.  Here I used multithreading to demonstrate the use of jthreads, queues, mutex, and counting_semaphores.  There is a separate thread that is waiting for responses from the message processor.  This thread sends the response back to the sender when a message is received.  Using this method of adding work to queues and having different threads do different work allows for more efficient processing from the user's point of view.  I can add more threads to process more request in parallel.  However, since this is a robot that is being controlled by the user, I only want one message at a time.  A move forward and a move backward request being processed simultaneously probably would not produce the desired results.

message:

The Message class is used to get information from the message buffer and used to generate the response.  This is the base class for all message types.  So far, only TestMessage has been implemented to inherit the Message class.  Eventually there will be message types for controlling the robot (move, sense, ???).  These types will help direct the request to the proper components.

MessageProcessor is the class used to process the message.  It is the class that removes the requests from the message queue and puts the response on the queue after the message has been processed.

MessageQueue is the queue manager class.  It does all of the queue manipulation.

Once I start simulating the robot functionality, there will be more components

I'm using cmake to build the simulator.  I normally build my own Makefiles, however it seems there are companies that want people who know how to use cmake.  Apparently, knowlege of make is not good enough anymore.  So, I'm using this project to learn cmake.  I'm not a big fan of all the extra files that are left lying all over the place with no easy way to clean them up.  I'm going to have to write a script to clean them up.
