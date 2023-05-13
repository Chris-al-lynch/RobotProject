from django.db import models

class Robot( models.Model ):
    name = models.CharField( max_length=128, primary_key=True )
    interfaceIPAddress = models.TextField()
    interfacePort = models.IntegerField()
    robotIPAddress = models.TextField()
    robotPort = models.IntegerField()
    description = models.TextField()

    def _str_( self ):
        return self.name