from django.db import models

class posicion (models.Model):
    name = models.CharField(max_length=100)
    angle = models.IntegerField()
    def __str__(self):
        return self.name
