from django.db import models

class Robot( models.Model ):
    name = models.CharField( max_length=128, primary_key=True )
    robotIPAddress = models.GenericIPAddressField()
    robotPort = models.IntegerField()
    description = models.TextField()

    def _str_( self ):
        return self.name