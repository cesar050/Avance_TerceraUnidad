from django.urls import path
from . import views

urlpatterns = [
    path('', views.Controles, name='Controles'),
    path('enviar/', views.enviar, name='enviar'),
]
