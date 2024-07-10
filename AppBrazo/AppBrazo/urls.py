from django.contrib import admin
from django.urls import path
from . import views
from django.conf import settings
from django.conf.urls.static import static

urlpatterns = [
    path('', views.InicioSesion, name='InicioSesion'),
    path('Materiales/', views.Materiales, name='Materiales'),
    path('CrearUsuario/', views.CrearUsuario, name='CrearUsuario'),
    path('home/', views.home, name='home'),
    path('admin/', admin.site.urls),
    path('Controles/', views.Controles, name='Controles'),

]


if settings.DEBUG:
    urlpatterns += static(settings.STATIC_URL, document_root=settings.STATIC_ROOT)