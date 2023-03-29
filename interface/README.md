Java concepts that can be found here: inheritance, Create your own Exceptions, sending messages through a TCP socket as a client, ByteBuffers, turning objects into byte arrays, 

This component is an interface between the webapp and the robot (simulator).  

This component will receive requests from the webapp and build an appropriate message and send it to the robot.

Since at this point I have no idea what the interface between the webapp and the interface is going to look like, I will start with building messages and sending them to the robot.  Later, I will define the interface between the webapp and this component.

Progress:

Docker image

The interface now has a Client that builds a test message, sends it to the simulator, receives a response from the simulator and exits.

Problems:  I believe I overcomplicated the endian code.  I had an issue with the server (simulator) receiving code in big endian when it was built to process everything in little endian.  The server then responded with a little endian response and the Java code was attempting to process big endian.  I believe I can remove the endian changes I made on the server side.  I can change the ByteBuffers to be little endian before processing any data and get rid of all the ByteOrder parameters throughout the message code.  I will do this in the next revision.