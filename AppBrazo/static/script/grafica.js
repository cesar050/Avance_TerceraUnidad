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
        const windowMinutes = 15;
        const now = new Date();
        const cutoffTime = now.getTime() - (windowMinutes * 60 * 1000);

        // Filtra los datos para mostrar solo los que estén dentro de la ventana de tiempo
        const filteredData = data.filter(dato => new Date(dato.timestamp).getTime() >= cutoffTime);

        // Limpia el gráfico existente si existe
        if (chart) {
            chart.destroy();
        }

        // Obtiene las etiquetas y datos para el gráfico de los datos filtrados
        const labels = filteredData.map(dato => new Date(dato.timestamp).toLocaleString());
        const datosBase = filteredData.map(dato => dato.base);
        const datosHombro = filteredData.map(dato => dato.hombro);
        const datosCodo = filteredData.map(dato => dato.codo);
        const datosPinza = filteredData.map(dato => dato.pinza);
        const datosMuñeca = filteredData.map(dato => dato.muñeca);
        const datosCamara = filteredData.map(dato => dato.camara);
        const datosArriba = filteredData.map(dato => dato.arriba);
        const datosAbajo = filteredData.map(dato => dato.abajo);
        const datosIzquierda = filteredData.map(dato => dato.izquierda);
        const datosDerecha = filteredData.map(dato => dato.derecha);

        // Crea una nueva instancia de Chart
        chart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: labels,
                datasets: [
                    {
                        label: 'Ángulo de la Base',
                        data: datosBase,
                        borderColor: 'rgb(193,166,166)',
                        backgroundColor: 'rgba(75, 192, 192, 0.2)',
                        borderWidth: 1
                    },
                    {
                        label: 'Ángulo del Hombro',
                        data: datosHombro,
                        borderColor: 'rgb(255,99,132)',
                        backgroundColor: 'rgba(255,99,132,0.2)',
                        borderWidth: 1
                    },
                    {
                        label: 'Ángulo del Codo',
                        data: datosCodo,
                        borderColor: 'rgb(75, 192, 192)',
                        backgroundColor: 'rgba(75, 192, 192, 0.2)',
                    },
                    {
                        label: 'Ángulo de la Pinza',
                        data: datosPinza,
                        borderColor: 'rgb(54,162,235)',
                        backgroundColor: 'rgba(54,162,235,0.2)',
                        borderWidth: 1
                    },
                    {
                        label: 'Ángulo de la Muñeca',
                        data: datosMuñeca,
                        borderColor: 'rgb(153,102,255)',
                        backgroundColor: 'rgba(153,102,255,0.2)',
                        borderWidth: 1
                    },
                    {
                        label: 'Ángulo de la Cámara',
                        data: datosCamara,
                        borderColor: 'rgb(255,206,86)',
                        backgroundColor: 'rgba(255,206,86,0.2)',
                        borderWidth: 1
                    },
                    {
                        label: 'Boton Arriba',
                        data: datosArriba,
                        borderColor: 'rgb(255,206,86)',
                        backgroundColor: 'rgba(255,206,86,0.2)',
                        borderWidth: 1
                    },
                    {
                        label: 'Boton Abajo',
                        data: datosAbajo,
                        borderColor: 'rgb(255,206,86)',
                        backgroundColor: 'rgba(255,206,86,0.2)',
                        borderWidth: 1
                    },
                    {
                        label: 'Boton Izquierda',
                        data: datosIzquierda,
                        borderColor: 'rgb(255,206,86)',
                        backgroundColor: 'rgba(255,206,86,0.2)',
                        borderWidth: 1
                    },
                    {
                        label: 'Boton Derecha',
                        data: datosDerecha,
                        borderColor: 'rgb(255,206,86)',
                        backgroundColor: 'rgba(255,206,86,0.2)',
                        borderWidth: 1
                    }
                ]
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
