#!/usr/bin/python3
##
# This class implements the functions I need to reset my internet connection.
#
# I'm having network issues.  I'm tired of my docker commands failing and
# having to manually reset my network and run it again.  So, I wrote some
# utilities to check my internet connection and reset it if needed.
#
# My network issues are due to an old out-dated possibly broken wireless card in
# my computer. I don't want to go buy a new one at the moment, so I thought I 
# would use this as an opportunity to show off some Linux stuff.
#
# TODO: Add better error handling when time allows.
#

from argparse import ArgumentParser
from os import read
from pty import spawn
from subprocess import Popen, PIPE, STDOUT
from time import sleep

# Constants
NMCLI = "/usr/bin/nmcli"

class InternetConnection():

   ##
   # Constructor
   #
   def __init__( self, wifiId=None ):
      self.wifiId = wifiId

   ##
   # Determine if computer is connected to the internet.
   #
   # @return True if connected, False if not
   #
   def isConnected( self ):
      proc = Popen( [NMCLI, "networking", "connectivity"], encoding='ascii', stdout=PIPE, stderr=STDOUT )
      output = proc.communicate()[0].strip()

      return (output == "full")
         
   ##
   # Reset the wifi card by turning it off, then back on.
   # TODO: Probably should add some error handling at some point.  Not important at the moment.
   #
   def resetWifi( self ):
      Popen( [NMCLI, "radio", "wifi", "off"] ).communicate()
      sleep( 3 )
      Popen( [NMCLI, "radio", "wifi", "on"] ).communicate()
      sleep( 3 )

   ##
   # Connect to the specified Wifi
   #
   # @param wifiId SSID for the connection to be made.
   #
   def connect( self, wifiID=None ):
      if( wifiID == None and self.wifiId == None ):
         raise Exception( "Must specify wifi ID" )
      
      myWifiID = self.wifiId
      
      if( wifiID ):
         myWifiID = wifiID

      # Make sure the connection is down befor trying to bring it up.
      Popen( [NMCLI, "connection", "down", myWifiID] ).communicate()

      proc = Popen( [NMCLI, "connection", "up", myWifiID], stdout=PIPE, stderr=STDOUT )

      output = proc.communicate()[0]

      if( proc.returncode != 0 ):
         raise Exception( "Failed to bring up connection for " + myWifiID + ": " + str( output ) )

   ##
   # Display all possible wifi connections as seen by the wifi card.
   #
   def displayPossibleWifiConnections( self ):
      spawn( [NMCLI, "-f", "ALL", "dev", "wifi"], lambda fd: read( fd, 1024 ) )

"""
from requests import head, ConnectionError
   This is the python interface for validating a connection.  However, I wanted to use
   nmcli for everything.  I left this here just for reference in case I need it in the future.

   def __init__( self, url="http://www.example.com", timeout=5 ):
      self.url     = url
      self.timeout = timeout

   def isConnected( self ):
      connected = True

      try:
        head( self.url, self.timeout )

      except ConnectionError:
        connected = False

      return connected
"""

####################################################################

def parseArgs():
   parser = ArgumentParser( prog = 'Internet.py', description = 'Manages wifi connection' )
   muxGroup = parser.add_mutually_exclusive_group()
   muxGroup.add_argument( "--reset", required=False, action='store_true', default=False )
   muxGroup.add_argument( "--show", required=False, action='store_true', default=False )
   muxGroup.add_argument( "--connect", required=False, action='store', default=None )
   muxGroup.add_argument( "--isConnected", required=False, action='store_true', default=False )
   # I got tired of resetting it by hand.  I'm going to run this in the background.
   muxGroup.add_argument( "--monitor", required=False, action='store_true', default=False )

   args  = parser.parse_args()

   cmd   = None
   parms = None

   if( args.reset ):
      cmd = "reset"
   elif( args.show ):
      cmd = "show"
   elif( args.connect ):
      cmd = "connect"
      parms = args.connect
   elif( args.isConnected ):
      cmd = "isConnected"
   elif( args.monitor ):
      cmd = "monitor"
   
   if( cmd == None ):
      parser.error( "Must specify reset|show|connect")
   
   return (cmd,parms)

def monitor():
   ic = InternetConnection()

   while True:
      if( ic.isConnected() ):
         sleep( 5 )
      else:
         ic.resetWifi()
         if( not ic.isConnected() ):
            ic.connect( "wifi2022" )

##
# I wanted to be able to run these commands from the command line, so I
# created a main here to allow me to do so easily.
#
def main():
   cmd, parms = parseArgs()

   ic = InternetConnection()

   if( parms ):
      ic = InternetConnection( parms )

   process = { "reset":ic.resetWifi, "show":ic.displayPossibleWifiConnections,
               "connect":ic.connect, "isConnected":ic.isConnected, "monitor":monitor }

   ec = process[cmd]()

   if( cmd == "isConnected" ):
      print( str( ec ) )
      if( ec ):
         ec = 0
      else:
         ec = 1

   exit( ec )

if( __name__ == '__main__' ):
   main()
