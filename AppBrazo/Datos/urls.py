from django.urls import path
from . import views

urlpatterns = [
    path('', views.Informacion, name='Informacion'),
    path('Integrantes/', views.Integrantes, name='Integrantes'),
    path('Documentacion/', views.Documentacion, name='Documentacion'),
    path('Materiales/', views.Materiales, name='Materiales'),
    path('MasInfo/', views.MasInfo, name='MasInfo'),
]
