const sliderCam = document.getElementById('camera');
const anguloCam = document.getElementById('anguloCam');

const sliderBas = document.getElementById('base');
const anguloBas = document.getElementById('anguloBas');

const sliderHom = document.getElementById('hombro');
const anguloHom = document.getElementById('anguloHom');

const sliderCod = document.getElementById('codo');
const anguloCod = document.getElementById('anguloCod');

const sliderMun = document.getElementById('muneca');
const anguloMun = document.getElementById('anguloMun');

const sliderPin = document.getElementById('pinza');
const anguloPin = document.getElementById('anguloPin');

const buttonForward = document.getElementById('forward');
const buttonBackward = document.getElementById('backward');

let directionValue = {
    forward: 0,
    backward: 0,
    left: 0,
    right: 0
};

const reset = document.getElementById('reset');

function fetchGet() {
    let formControl = new FormData(document.getElementById('form-control'));
    formControl.append('camera', sliderCam.value);
    formControl.append('base', sliderBas.value);
    formControl.append('hombro', sliderHom.value);
    formControl.append('codo', sliderCod.value);
    formControl.append('muneca', sliderMun.value);
    formControl.append('pinza', sliderPin.value);
    formControl.append('forward', directionValue.forward);
    formControl.append('backward', directionValue.backward);


    const esp32_ip = document.getElementById('esp32_ip').value;
    formControl.append('esp32_ip', esp32_ip);

    let parametros = new URLSearchParams(formControl).toString();
    let url = `http://127.0.0.1:8000/control/?${parametros}`;

    fetch(url, {
        method: 'GET',
        headers: {
            'Accept': 'text/plain',
        }
    }).then(response => {
        if (!response.ok) {
            throw new Error('Network response was not ok ' + response.statusText);
        }
        return response.text();
    }).then(data => {
        console.log('Success:', data);
    }).catch(error => {
        console.error('Error:', error);
    });

    console.log(url);
}

let sliders = [sliderCam, sliderBas, sliderHom, sliderCod, sliderMun, sliderPin];

function updateSliderValue(slider, angulo) {
    slider.addEventListener('input', function() {
        angulo.textContent = slider.value;
    });
    angulo.textContent = slider.value;
}

updateSliderValue(sliderCam, anguloCam);
updateSliderValue(sliderBas, anguloBas);
updateSliderValue(sliderHom, anguloHom);
updateSliderValue(sliderCod, anguloCod);
updateSliderValue(sliderMun, anguloMun);
updateSliderValue(sliderPin, anguloPin);

sliders.forEach(slider => {
    slider.addEventListener('input', function() {
        fetchGet();
    });
});

reset.addEventListener('click', function() {
    sliders.forEach(slider => {
        slider.value = 90;
        document.getElementById(`angulo${slider.id.charAt(0).toUpperCase() + slider.id.slice(1)}`).textContent = 90;
    });
    fetchGet();
});

buttonForward.addEventListener('mousedown', function() {
    directionValue.forward = 1;
    fetchGet();
});
buttonForward.addEventListener('mouseup', function() {
    directionValue.forward = 0;
    fetchGet();
});

buttonBackward.addEventListener('mousedown', function() {
    directionValue.backward = 1;
    fetchGet();
});
buttonBackward.addEventListener('mouseup', function() {
    directionValue.backward = 0;
    fetchGet();
});


document.addEventListener('keydown', function(event) {
    switch(event.key) {
        case 'w':
        case 'W':
            directionValue.forward = 1;
            fetchGet();
            break;
        case 's':
        case 'S':
            directionValue.backward = 1;
            fetchGet();
            break;
        case 'a':
        case 'A':
            directionValue.left = 1;
            fetchGet();
            break;
        case 'd':
        case 'D':
            directionValue.right = 1;
            fetchGet();
            break;
    }
});

document.addEventListener('keyup', function(event) {
    switch(event.key) {
        case 'w':
        case 'W':
            directionValue.forward = 0;
            fetchGet();
            break;
        case 's':
        case 'S':
            directionValue.backward = 0;
            fetchGet();
            break;
        case 'a':
        case 'A':

    }
});
