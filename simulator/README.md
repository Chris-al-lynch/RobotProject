This component is a simulator for a robot.  I don't have a real robot at the moment, so I am going to simulate a robot with software.  I would eventually like to hook up a webcam and simulate the robot having a camera and stream it back to the client/controller.

Progress:

This component was going to be one of the last ones I worked on.  However, I have a coding interview in C++ coming up and need some C++ practice.  So, I'm going to do some work on it now.

I have written a TCP server which receives messages from a client (yet to be written), processes the message, and returns a response back to the sender.  I will describe each of the classes used to develop this code.  Keep in mind that there is no client yet, which means this code has not been tested yet.  I still need to go through and comment the code, write a test client, and test everything.  Eventually, the client will be written in Java as the interface component.  That will be done after I finish with this component and verify everything is working.

utilities:

Exceptions.cpp contains multiple exception classes.  I like to create my own exceptions so it is easier to see which component failed.

Logger - I always create a Logger class for most big projects.  It makes it easier to change how things are logged.  Right now, everything is going to standard out.  If later I want to send things to a file or both to a file and standard out, I only have to change it in the Logger class.


server:

The Server class implements a TCP server.  The address and port on which the server listens are configurable at start time.  It would be interesting to make this configurable at run time.  Maybe I can add this enhancement in the future.  I havent finished the stop code yet because I am killing the process to kill the server.  I will have a clean shutdown by the time I'm finished.  The server waits for a message on the configured port.  Once one is received it loads the message in a buffer and throws it on a queue.  Here I used multithreading to demonstrate the use of jthreads, queues, mutex, and counting_semaphores.  Again, I could have messed something up.  I haven't tested it yet.  Maybe wait for the final code to actually see the usage.  There is a separate thread that is waiting for responses from the message processor.  This thread sends the response back to the sender when a message is received.  Using this method of adding work to queues and having different threads do different work allows for more efficient processing from the user's point of view.  I can add more threads to process more request in parallel.  However, since this is a robot that is being controlled by the user, I only want one message at a time.  A move forward and a move backward request being processed simultaneously probably would not produce the desired results.

message:

The Message class is used to get information from the message buffer and used to generate the response.  This is the base class for all message types.  So far, only TestMessage has been implemented to inherit the Message class.  Eventually there will be message types for controlling the robot (move, sense, ???).  These types will help direct the request to the proper components.

MessageProcessor is the class used to process the message.  It is the class that removes the requests from the message queue and puts the response on the queue after the message has been processed.

MessageQueue is the queue manager class.  It does all of the queue manipulation.


Once I start simulating the robot functionality, there will be more components

I'm using cmake to build the simulator.  I normally build my own Makefiles, however it seems there are companies that want people who know how to use cmake.  Apparently, knowlege of make is not good enough anymore.  So, I'm using this project to learn cmake.  I'm not a big fan of all the extra files that are left lying all over the place with no easy way to clean them up.  I'm going to have to write a script to clean them up.
