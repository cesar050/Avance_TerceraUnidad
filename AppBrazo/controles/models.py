from django.db import models

class ServoData(models.Model):
    esp32_ip = models.CharField(max_length=15)
    timestamp = models.DateTimeField(auto_now_add=True)
    base = models.IntegerField()
    hombro = models.IntegerField()
    muneca = models.IntegerField()
    codo = models.IntegerField()
    pinza = models.IntegerField()
    camera = models.IntegerField()
    forward = models.IntegerField()
    backward = models.IntegerField()
    left = models.IntegerField()
    right = models.IntegerField()

    def __str__(self):
        return f'ServoData at {self.timestamp}'
