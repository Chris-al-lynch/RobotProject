from rest_framework import viewsets

from rest_framework import status
from rest_framework.decorators import api_view
from rest_framework.response import Response

from .models import Robot
from .serializers import RobotSerializer

class RobotView(  viewsets.ModelViewSet ):
    serializer_class = RobotSerializer
    queryset = Robot.objects.all()

class RobotRest():
    def __init__( self, request, pk=None ):
        self.restFunc = { 'GET': self.get,
                          'POST': self.post,
                          'PUT': self.put,
                          'DELETE': self.delete }
        self.request = request
        self.pk = pk

        if( request.method not in self.restFunc ):
            raise Exception
 
    def execute( self ):
        return self.restFunc[self.request.method]()
    
    def getRobotConfig( self ):
        try:
            return Robot.objects.get( pk=self.pk )
        except Robot.DoesNotExist:
            print( "The item " + str( self.pk ) + " was not found." )
            return Response( status=status.HTTP_404_NOT_FOUND )

    def get( self ):
        print( "get(): pk=" + str( self.pk ) )
        if( self.pk == None ):
            robotList = Robot.objects.all()
        else:
            robotList = self.getRobotConfig()
            if( isinstance( robotList, Response ) ):
                print( "get(): returning with error." )
                return robotConfig

        print( "robotList = " + str( robotList ) )
        serializer = RobotSerializer( robotList,
                                      many=True )

        print( "get(): returning: " + str( serializer.data ) )
        return Response( serializer.data )

    def post( self ):
        serializer = RobotSerializer( data=self.request.data )
        if( serializer.is_valid() ):
            serializer.save()
            return Response( status=status.HTTP_201_CREATED )
        print( "Errors = " + str( serializer.errors ) + ", data = " + str( serializer.data ) )
        return Response( serializer.errors, status=status.HTTP_400_BAD_REQUEST )

    def put( self ):
        robotConfig = self.getRobotConfig()

        if( isinstance( robotConfig, Response ) ):
            return robotConfig
        
        serializer = RobotSerializer( robotConfig, data=self.request.data,
                                      context={'request': self.request} )
        if( serializer.is_valid() ):
            serializer.save()
            return Response( status=status.HTTP_204_NO_CONTENT )
        return Response( serializer.errors, status=status.HTTP_400_BAD_REQUEST )

    def delete( self ):
        print( "delete(): self.pk = " + str( self.pk ) )
        robotConfig = self.getRobotConfig()
        print( "delete(): robotConfig = " + str( robotConfig ) )

        if( isinstance( robotConfig, Response ) ):
            return robotConfig
        
        robotConfig.delete()
        return Response( status=status.HTTP_204_NO_CONTENT )

@api_view( ['GET', 'POST', 'PUT', 'DELETE'] )
def robotConfig( request, pk=None ):
    robotRest = RobotRest( request, pk )
    return robotRest.execute()
