#!/usr/bin/python3

##
# The purpose of this script is mainly to show that I can write Python code.
# It wraps the docker command line routines that I use for running and stopping
# this project.  This script is written in the procedural programming style.
# See Internet.py for my objected oriented Python.
# 

from argparse import ArgumentParser
from os import read
from pty import spawn
from sys import exit
from Internet import InternetConnection

##
# Execute a specified system command.  Will send output to stdout real-time.
#
# @param cmdParmList The command as a list.  For example: ["cmd", "parm1", value, "parm2"]
#
# @return Exit code from command execution.
#
def executeCmd( cmdParmList ):
    ##
    # I had all kinds of code here using subprocess.Popen().  I wanted to
    # print the output in color just like when you executed the command on
    # the command line.  However, Popen cannot print in color.  So, I scoured
    # the web and found this simple solution.  Actually, the solution I found
    # created a separate callback read function.  I didn't need the extra
    # information it was storing, so I created the simple lambda function.
    #
    return spawn( cmdParmList, lambda fd: read( fd, 1024 ) )

    # The problem with spawn 

##
#  Remove the specified image
# 
# @param images A comma separated list of images 
#
def removeImage( images ):
   rc = 0
   if( "," in images ):
      imageList = images.split( ',' )
   else:
      imageList = [images]

   # Loop through each image in the list
   for image in imageList:
      # Remove the image from docker.
      rc = executeCmd( ["/usr/local/bin/docker", "image", "rm", image] )

   return rc

##
# Use 'docker compose up' to execute the docker-compose.yml file which builds and runs
# the images for this project.
#
# @param unused Unused parameter.  Exists for function lookup table in main.
#
def start( unused ):
   # Before starting, I need to make sure the internet is running.  If it is not,
   # I need to attempt to reset it.  I have a flakey wifi card in my PC.  Sometimes
   # I lose connection, and I have to disable the card, and re-enable it to get my
   # wifi connection back.  See the InternetConnection module for more information.
   ic = InternetConnection()

   # Only do this if not connected.
   if( not ic.isConnected() ):
      #  Reset the wifi card.  It should auto-connect to the internet.
      ic.resetWifi()

      # Sometimes my card does not automatically connect.  I have to manually reconnect
      # when this happens.
      if( not ic.isConnected() ):
         ic.connect( "wifi2022" )
         
   # Create all of the images and the container: docker compose up
   return executeCmd( ["/usr/local/bin/docker", "compose", "up"] )

##
# Use 'docker compose down' to shutdown the images if they are running and remove the container.
# All images will remain.
#
# @param unused Unused parameter.  Exists for function lookup table in main.
#
def stop( unused ):
   # Stop and remove the container: docker compose down
   return executeCmd( ["/usr/local/bin/docker", "compose", "down"] )

##
# Parse the arguments
#
# @return tuple containing command (start or stop) and parameters which is None by default
#
def parseArgs():
   parser = ArgumentParser( prog = 'run.py', description = 'stops and starts project' )
   muxGroup = parser.add_mutually_exclusive_group()
   muxGroup.add_argument( '--start', required=False, action='store_true', help="Start the docker container as specified in docker_compose.yml" )
   muxGroup.add_argument( '--stop', required=False, action='store_true',  help="Stop the docker container" )
   parser.add_argument( '--removeImage', '--ri', required=False, action='store', default=None,
                         help="Optional parameter to remove specified image.  Only works with stop." )

   args = parser.parse_args()

   if( args.start and args.removeImage ):
      parser.error( "Cannot pass removeImage with start." )

   if( not args.start and not args.stop and not args.removeImage ):
      parser.error( "Must specify a valid parameter." )

   cmd   = None
   parms = args.removeImage

   if( args.start ):
      cmd = "start"

   if( args.stop ):
      cmd = "stop"
    
   return (cmd, parms)

##
# Main function which processes the arguments a calls the appropriate function.
#
# run.py <--start|--stop> [--removeImage|--ri <image to remove>]
#
def main():
   process = { "start":start, "stop":stop, "removeImage":removeImage }
   cmd, parms = parseArgs()

   rc = 0

   if( cmd ):
      rc = process[cmd]( parms )

   if( (rc == 0) and parms ):
      rc = process["removeImage"]( parms )

   exit( rc )

if( __name__ == '__main__' ):
   main()