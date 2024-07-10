from django import forms

class InicioSecionForm(forms.Form):
    correo = forms.EmailField(widget=forms.EmailInput(attrs={'class': 'form-control'}))
    contrasena = forms.CharField(widget=forms.PasswordInput(attrs={'class': 'form-control'}))