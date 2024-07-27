from django.shortcuts import render
from django.http import HttpResponse
import requests
from .models import ServoData

def Controles(request):
    esp32_ip = request.GET.get('esp32_ip', '')

    required_params = ['base', 'hombro', 'muneca', 'codo', 'pinza', 'camera', 'forward', 'backward', 'left', 'right']

    if esp32_ip and all(param in request.GET for param in required_params):
        params = {param: request.GET[param] for param in required_params}

        port = '80'
        url = f'http://{esp32_ip}:{port}/'
        try:
            response = requests.get(url, params=params)
            response.raise_for_status()
        except requests.exceptions.RequestException as e:
            return render(request, 'Controles.html', {
                'error_message': f'Error en la conexión con el ESP32: {e}',
                'esp32_ip': esp32_ip
            })

        ServoData.objects.create(
            esp32_ip=esp32_ip,
            base=params['base'],
            hombro=params['hombro'],
            muneca=params['muneca'],
            codo=params['codo'],
            pinza=params['pinza'],
            camera=params['camera'],
            forward=params['forward'],
            backward=params['backward'],
            left=params['left'],
            right=params['right']
        )

        return render(request, 'Controles.html', {
            'success_message': 'Servomotores movidos',
            'esp32_ip': esp32_ip
        })

    return render(request, 'Controles.html', {
        'esp32_ip': esp32_ip
    })
