from django.urls import path
from . import views

urlpatterns = [
    path('', views.Controles, name='Controles'),
    path('estadisticas/', views.Estadisticas, name='Estadisticas'),
    path('obtener_datos_servo/', views.obtener_datos_servo, name='obtener_datos_servo'),
]
