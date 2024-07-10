from django import forms
from django.core.exceptions import ValidationError
from .models import Usuarios

class UsuarioForm(forms.ModelForm):
    class Meta:
        model = Usuarios
        fields = ['nombre', 'correo', 'contrasena', 'telefono']
        widgets = {
            'nombre': forms.TextInput(attrs={'class': 'form-control'}),
            'correo': forms.EmailInput(attrs={'class': 'form-control'}),
            'contrasena': forms.PasswordInput(attrs={'class': 'form-control'}),
            'telefono': forms.TextInput(attrs={'class': 'form-control'}),
        }
        labels = {
            'nombre': 'Nombre',
            'correo': 'Correo',
            'contrasena': 'Contraseña',
            'telefono': 'Telefono',
        }
        help_texts = {
            'nombre': 'Ingrese su nombre.',
            'correo': 'Ingrese su correo.',
            'contrasena': 'Ingrese su contraseña.',
            'telefono': 'Ingrese su telefono.',
        }
        error_messages = {
            'nombre': {
                'required': 'El nombre es requerido.',
            },
            'correo': {
                'required': 'El correo es requerido.',
            },
            'contrasena': {
                'required': 'La contraseña es requerida.',
            },
            'telefono': {
                'required': 'El telefono es requerido.',
            },

        }

    def clean_correo(self):
        correo = self.cleaned_data.get('correo')
        if Usuarios.objects.filter(correo=correo).exists():
            raise forms.ValidationError("Este usuario ya existe.")
        return correo