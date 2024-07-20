from django.shortcuts import render

def Informacion(request):
    return render(request, 'Informacion.html')

def Integrantes(request):
    return render(request, 'INFO/Integrantes.html')

def Documentacion(request):
    return render(request, 'INFO/Documentacion.html')

def Materiales(request):
    return render(request, 'INFO/Materiales.html')

def MasInfo(request):
    return render(request, 'INFO/MasInfo.html')
