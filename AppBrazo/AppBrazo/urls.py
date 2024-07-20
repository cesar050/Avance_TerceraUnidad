from django.contrib import admin
from django.urls import path, include
from . import views
from django.conf import settings
from django.conf.urls.static import static

urlpatterns = [
    path('', views.InicioSesion, name='InicioSesion'),
    path('Informacion/', include('Datos.urls')),
    path('CrearUsuario/', views.CrearUsuario, name='CrearUsuario'),
    path('home/', views.home, name='home'),
    path('admin/', admin.site.urls),
    path('control/', include('controles.urls')),
]


if settings.DEBUG:
    urlpatterns += static(settings.STATIC_URL, document_root=settings.STATIC_ROOT)