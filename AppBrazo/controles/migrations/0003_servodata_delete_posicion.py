# Generated by Django 5.0.6 on 2024-07-27 21:17

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('controles', '0002_rename_servo_posicion'),
    ]

    operations = [
        migrations.CreateModel(
            name='ServoData',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('esp32_ip', models.CharField(max_length=15)),
                ('timestamp', models.DateTimeField(auto_now_add=True)),
                ('base', models.IntegerField()),
                ('hombro', models.IntegerField()),
                ('muneca', models.IntegerField()),
                ('codo', models.IntegerField()),
                ('pinza', models.IntegerField()),
                ('camera', models.IntegerField()),
                ('forward', models.IntegerField()),
                ('backward', models.IntegerField()),
                ('left', models.IntegerField()),
                ('right', models.IntegerField()),
            ],
        ),
        migrations.DeleteModel(
            name='posicion',
        ),
    ]
