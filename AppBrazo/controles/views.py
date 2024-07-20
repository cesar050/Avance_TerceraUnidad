from django.shortcuts import render
from django.http import JsonResponse

def Controles(request):
    return render(request, 'Controles.html')

def enviar(request):
    if request.method == 'GET':
        pinza_brazo = request.GET.get('pinza_brazo', None)
        # Aquí puedes agregar el código para enviar `pinza_brazo` al ESP32
        return JsonResponse({'status': 'success', 'pinza_brazo': pinza_brazo})
    return JsonResponse({'status': 'error'}, status=400)
