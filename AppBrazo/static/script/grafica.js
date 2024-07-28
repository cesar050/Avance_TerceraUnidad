document.addEventListener('DOMContentLoaded', function() {
    let chart;

    function obtenerDatos() {
        fetch('/control/obtener_datos_servo/')
            .then(response => response.json())
            .then(data => {
                actualizarGrafico(data);
            })
            .catch(error => console.error('Error al obtener los datos:', error));
    }

    function actualizarGrafico(data) {
        const ctx = document.getElementById('grafico-base').getContext('2d');

        // Limpia el gráfico existente si existe
        if (chart) {
            chart.destroy();
        }

        const labels = data.map(dato => new Date(dato.timestamp).toLocaleString());
        const datosBase = data.map(dato => dato.base);

        // Crea una nueva instancia de Chart
        chart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: labels,
                datasets: [{
                    label: 'Ángulo de la Base',
                    data: datosBase,
                    borderColor: 'rgba(75, 192, 192, 1)',
                    backgroundColor: 'rgba(75, 192, 192, 0.2)',
                    borderWidth: 1
                }]
            },
            options: {
                responsive: true,
                scales: {
                    x: {
                        beginAtZero: true
                    },
                    y: {
                        beginAtZero: true
                    }
                }
            }
        });
    }

    function enviarDatoFinal(sliderId, valor) {
        fetch(`/control/enviar_dato_servo/${sliderId}/`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ valor: valor })
        })
        .then(response => response.json())
        .then(data => {
            console.log(`Dato final del ${sliderId} enviado:`, data);
            obtenerDatos(); // Actualiza el gráfico con el nuevo dato
        })
        .catch(error => console.error('Error al enviar el dato:', error));
    }

    // Agrega eventos a los sliders
    const sliders = document.querySelectorAll('.slider');
    sliders.forEach(slider => {
        slider.addEventListener('change', (event) => {
            const sliderId = event.target.id;
            const valor = event.target.value;
            enviarDatoFinal(sliderId, valor);
        });
    });

    obtenerDatos(); // Llama a la función para cargar los datos al cargar la página

    // Actualizar cada 30 segundos
    setInterval(obtenerDatos, 30000);
});
