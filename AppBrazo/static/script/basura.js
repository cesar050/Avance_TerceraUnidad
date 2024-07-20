document.addEventListener('DOMContentLoaded', () => {
    const valor = document.getElementById('pinza_brazo');
    let form = document.getElementById("sliderForm");

    if (valor && form) {
        valor.addEventListener('input', () => {
            let formData = new FormData(form);
            let parametros = new URLSearchParams(formData);

            let url = 'enviar/?' + parametros.toString();
            fetch(url, {
                method: 'GET',
                headers: {
                    'Content-Type': 'application/json',
                }
            })
                .then(response => response.json())
                .then(data => console.log(data))
                .catch(error => console.error('Error:', error));
        });

        console.log(valor);
    } else {
        console.error('Elementos no encontrados');
    }
});
