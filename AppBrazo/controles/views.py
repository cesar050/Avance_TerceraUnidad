from django.shortcuts import render
from django.http import HttpResponse
import requests

def Controles(request):
    esp32_ip = 'http://192.168.3.159'

    required_params = ['base', 'hombro', 'muneca', 'codo', 'pinza', 'camera', 'forward', 'backward', 'left', 'right']

    modified_params = {param: request.GET[param] for param in required_params if param in request.GET and request.GET[param]}

    if modified_params:
        query_string = '&'.join([f'{param}={value}' for param, value in modified_params.items()])
        url = f'{esp32_ip}/?{query_string}'

        try:
            response = requests.get(url)
            response.raise_for_status()
        except requests.exceptions.RequestException as e:
            return render(request, 'Controles.html', {
                'error_message': f'Error en la conexión con el ESP32: {e}'
            })

        for param, value in modified_params.items():
            print(f'Moviendo {param} a {value}')

        # Guardar los datos en la base de datos
        from .models import ServoData
        ServoData.objects.create(
            esp32_ip=esp32_ip,
            base=modified_params.get('base', ''),
            hombro=modified_params.get('hombro', ''),
            muneca=modified_params.get('muneca', ''),
            codo=modified_params.get('codo', ''),
            pinza=modified_params.get('pinza', ''),
            camera=modified_params.get('camera', ''),
            forward=modified_params.get('forward', ''),
            backward=modified_params.get('backward', ''),
            left=modified_params.get('left', ''),
            right=modified_params.get('right', '')
        )

        moved_params = ', '.join([f'{param}: {value}' for param, value in modified_params.items()])
        response_message = f' {moved_params}'
        return HttpResponse(response_message, content_type='text/plain')

    return render(request, 'Controles.html')
