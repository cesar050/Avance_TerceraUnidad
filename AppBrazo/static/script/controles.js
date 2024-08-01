const sliders = {
    camera: { slider: document.getElementById('camera'), angle: document.getElementById('anguloCam') },
    base: { slider: document.getElementById('base'), angle: document.getElementById('anguloBas') },
    hombro: { slider: document.getElementById('hombro'), angle: document.getElementById('anguloHom') },
    codo: { slider: document.getElementById('codo'), angle: document.getElementById('anguloCod') },
    muneca: { slider: document.getElementById('muneca'), angle: document.getElementById('anguloMun') },
    pinza: { slider: document.getElementById('pinza'), angle: document.getElementById('anguloPin') }
};

const directionValue = {
    forward: 0,
    backward: 0,
    left: 0,
    right: 0
};

const buttonForward = document.getElementById('forward');
const buttonBackward = document.getElementById('backward');
const reset = document.getElementById('reset');

function fetchGet() {
    let formControl = new FormData();
    for (let key in sliders) {
        formControl.append(key, sliders[key].slider.value);
    }
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

function updateSliderValue(sliderObj) {
    sliderObj.slider.addEventListener('input', function() {
        sliderObj.angle.textContent = sliderObj.slider.value;
        fetchGet();
    });
    sliderObj.angle.textContent = sliderObj.slider.value;
}

for (let key in sliders) {
    updateSliderValue(sliders[key]);
}

reset.addEventListener('click', function() {
    for (let key in sliders) {
        sliders[key].slider.value = 90;
        sliders[key].angle.textContent = 90;
    }
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
    switch(event.key.toLowerCase()) {
        case 'w':
            directionValue.forward = 1;
            fetchGet();
            break;
        case 's':
            directionValue.backward = 1;
            fetchGet();
            break;
        case 'a':
            directionValue.left = 1;
            fetchGet();
            break;
        case 'd':
            directionValue.right = 1;
            fetchGet();
            break;
    }
});

document.addEventListener('keyup', function(event) {
    switch(event.key.toLowerCase()) {
        case 'w':
            directionValue.forward = 0;
            fetchGet();
            break;
        case 's':
            directionValue.backward = 0;
            fetchGet();
            break;
        case 'a':
            directionValue.left = 0;
            fetchGet();
            break;
        case 'd':
            directionValue.right = 0;
            fetchGet();
            break;
    }
});
