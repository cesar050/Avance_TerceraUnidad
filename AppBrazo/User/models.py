from django.db import models
from django.contrib.auth.hashers import make_password
from django.contrib import admin

class User(models.Model):
    nombre = models.CharField(max_length=100)
    apellido = models.CharField(max_length=100)
    correo = models.EmailField(max_length=100, unique=True)
    contrasena = models.CharField(max_length=15)
    telefono = models.CharField(max_length=10)

    def save(self, *args, **kwargs):
        self.contrasena = make_password(self.contrasena)
        super().save(*args, **kwargs)

    def __str__(self):
        return self.nombre

    class Meta:
        abstract = True

class Usuarios(User):
    access_db = models.BooleanField(default=True)

class Visitante(User):
    pass

admin.site.register(Usuarios)
