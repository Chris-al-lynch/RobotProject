 This project is a webapp that is a robot controller.  I don't actually have a robot to control at the moment, so I will be developing a robot simulator that will make the webapp believe it is talking to a real robot.

 There are 4 major components in this project:

 1) webapp - This is the front-end user component.  I am new to webapp development, meaning all I have done at this point is a react tutorial and a react-route tutorial.  I will be learning as I go, but plan on being an expert by the time I'm finished.  This code will be developed using Node.js with React.  I am starting with the webapp component.  See the README.md file in the webapp directory for more information and progress.  I have some initial code here, but I have moved to other components to practice for job interviews

 2) mysql - This is the storage for the webapp.  It will store the configuration information that will be used by the robot interface code.

 3) interface - This is the interface between the webapp code and the robot itself.  It will access the database to retrieve configuration information as well.  I will develop this code using Java and hopefully find a reason to add some JNI with C++.  I have started on this because I have a Java coding interview coming up soon.  I am starting with developing the client to talk with the C++ server which I already developed.  I will move forward from there.

 4) simulator - This code will simulate a real robot.  This includes sensors and motors.  One of my goals here is to hookup a webcam and simulate a camera on the robot and attempt to stream the image back to the webapp.  This code will be written in C++.  I have a C++ server written along with a test client.  See the Readme.md file for more information.

 I have developed some simple Python scripts to do some simple tasks.  There is a run.py script which does all of the docker stuff so I don't have to remember how to do the docker commands.  There is a removeCmakeFiles.py script to cleanup everything after cmake leaves its mess behind.  And there is an Internet.py which I created due to a faulty Wifi card in my computer which I no longer need because I went out and bought a new switch and am not hard-wired with my PC.

I will be using separate Docker containers for each component.  I am also new to Docker and container development.  I was asked about container development in an interview and had no idea what they were talking about.  I have always worked in a big company with access to any hardware I wanted.  We never had a need for containers.  The closest we got was using virtual machines.  So, I did a Docker tutorial, and now I'm ready to be an expert with Docker containers.  I have created a YAML file (docker-compose.yml) for bringing up all of the containers together.

As I said, I am new to webapp development.  At this point I have done a react tutorial and a react-router turorial.  There's a good chance that I will do more tutorials during the development of this project.  I am using yarn with Node.js.  I used npm during my tutorials, but I read somewhere that yarn is faster because it does things in parallel.  I haven't done a lot of research to actually know which is better, but I'll use yarn for now. I used the following command to create the webapp directory:

yarn create react-app webapp

 With that said, I am expecting this project to take a long time.  I'm not sure how much time I will have to spend on it each day.  I will check in changes as I get them working.  To start, I will check in my Docker files and a hello world for each component.

Any constructive advice is appreciated.

Wish me luck.

IMPORTANT NOTE:

If you are going to try this out, I have a couple of files I chose not to include in the repository that are needed for the Dockerfiles to work for the interface and simulator components.  The interface component is expecting ant.tar.gz which I did not check in.  The simulator component is expecting cmake.tar.gz.  When unpacked, ant.tar.gz looks like this:

ant
 |
 ---apache-ant-1.10.13/
 |
 ---latest

 The subdirectory apache-ant-1.10.13 is downloaded directly from Apache's binary page for Ant.  The file "latest", is simply a symbolic link to apache-ant-1.10-13.  

 The file cmake.tar.gz follows the same pattern.  When unpacked, cmake.tar.gz looks like this:

 cmake
   |
   ---cmake-3.26.0-rc5-linux-x86_64/
   |
   ---latest

 The subdirectory cmake-3.26.0-rc5-linux-x86_64 is downloaded directly from the binary page for cmake.  The file "latest", is simply a symbolic link to cmake-3.26.0-rc5-linux-x86_64.  

 Again, if you want to use the Docker files, you'll have to generate these tar.gz files.  

