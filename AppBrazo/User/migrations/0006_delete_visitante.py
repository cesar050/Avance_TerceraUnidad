# Generated by Django 5.0.6 on 2024-07-01 16:53

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('User', '0005_visitante_delete_administrador_usuario_access_db'),
    ]

    operations = [
        migrations.DeleteModel(
            name='Visitante',
        ),
    ]
