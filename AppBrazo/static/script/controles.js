const valor = document.getElementById('pinza_brazo');
let form = document.getElementById("sliderForm");

valor.addEventListener('input', (e) => {
    // Crea una instancia de FormData a partir del formulario
    let formData = new FormData(form);
    // Convierte FormData a URLSearchParams para construir la URL con los parámetros
    let parametros = new URLSearchParams(formData);

    // Construye la URL con los parámetros del formulario
    let url = 'http://127.0.0.1:8000/control/enviar/?' + parametros.toString();
    console.log('URL:', url);  // Añadido para depuración

    // Realiza la solicitud GET
    fetch(url, {
        method: 'GET',
        headers: {
            'Content-Type': 'application/json',  // Aunque no es necesario para GET, puedes dejarlo
        }
    })
        .then(response => {
            // Verifica el código de estado de la respuesta
            if (!response.ok) {
                throw new Error('Network response was not ok ' + response.statusText);
            }
            return response.json();
        })
        .then(data => {
            // Procesa los datos recibidos
            console.log('Data received:', data);
        })
        .catch(error => {
            // Maneja errores en la solicitud
            console.error('Error:', error);
        });
});
