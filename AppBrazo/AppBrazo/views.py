from django.shortcuts import render, redirect
from User.forms import UsuarioForm
from inicioSesion.forms import InicioSecionForm
from User.models import Usuarios
from django.contrib.auth.hashers import check_password


def InicioSesion(request):
    if request.method == 'POST':
        form = InicioSecionForm(request.POST)
        if form.is_valid():
            correo = form.cleaned_data['correo']
            contrasena = form.cleaned_data['contrasena']
            try:
                user = Usuarios.objects.get(correo=correo)
                if check_password(contrasena, user.contrasena):
                    request.session['user_id'] = user.id
                    return redirect('home')
                else:
                    form.add_error(None, 'Contraseña incorrecta')
            except Usuarios.DoesNotExist:
                form.add_error(None, 'Correo no registrado')
    else:
        form = InicioSecionForm()
    return render(request, 'InicioSecion.html', {'form': form})

def CrearUsuario(request):
    if request.method == 'POST':
        form = UsuarioForm(request.POST)
        if form.is_valid():
            form.save()
            return redirect('home')
        else:
            print(form.errors)
    else:
        form = UsuarioForm()
    return render(request, 'CrearUsuario.html', {'form': form})

def home(request):
    return render(request, 'home.html')


