#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "HX711.h"  // Librería para el sensor de peso
#include <AccelStepper.h>  // Librería para los motores paso a paso

// Definir los límites del servo
#define SERVOMIN  150 // Min pulso
#define SERVOMAX  600 // Max pulso

// Pines para el motor paso a paso
#define DIR_PIN 12
#define STEP_PIN 14

class Articulacion {
private:
    Adafruit_PWMServoDriver* pwm;
    int canal;
    int posInicial;
    int posNueva;
    int velocidad;
    bool enMovimiento;
    unsigned long tiempoInicio;
    int paso;
    int destino;

public:
    Articulacion(Adafruit_PWMServoDriver* _pwm, int _canal, int _posInicial = 0, int _velocidad = 20) {
        pwm = _pwm;
        canal = _canal;
        posInicial = _posInicial;
        posNueva = _posInicial;
        velocidad = _velocidad;
        enMovimiento = false;
    }

    void inicializar() {
        mover(posInicial);
    }

    void mover(int angulo) {
        if (angulo < 0) angulo = 0;
        if (angulo > 180) angulo = 180;
        posNueva = angulo;
        if (posInicial != posNueva && !enMovimiento) {
            enMovimiento = true;
            paso = (posNueva > posInicial) ? 1 : -1;
            destino = posNueva;
            tiempoInicio = millis();
        }
    }

    void actualizar() {
        if (enMovimiento) {
            unsigned long tiempoActual = millis();
            unsigned long tiempoTranscurrido = tiempoActual - tiempoInicio;

            if (tiempoTranscurrido >= velocidad) {
                posInicial += paso;
                if ((paso > 0 && posInicial >= destino) || (paso < 0 && posInicial <= destino)) {
                    posInicial = destino;
                    enMovimiento = false;
                }
                int pulse = map(posInicial, 0, 180, SERVOMIN, SERVOMAX);
                pwm->setPWM(canal, 0, pulse);
                tiempoInicio = tiempoActual;
            }
        }
    }
};

class MotorPaso {
private:
    AccelStepper stepper;

public:
    MotorPaso(int dirPin, int stepPin) : stepper(AccelStepper::DRIVER, stepPin, dirPin) {
        stepper.setMaxSpeed(1000);  // Máxima velocidad
        stepper.setAcceleration(500);  // Aceleración
    }

    void mover(int pasos) {
        stepper.moveTo(pasos);
    }

    void actualizar() {
        stepper.run();
    }
};

class SensorPeso {
private:
    HX711 bascula;

public:
    SensorPeso(int pin_dt, int pin_sck) {
        bascula.begin(pin_dt, pin_sck);
    }

    void tarar() {
        bascula.tare();
    }

    float obtenerPeso() {
        return bascula.get_units(10);
    }
};
class BrazoRobotico {
private:
    String nombre;
    Articulacion camera;
    Articulacion base;
    Articulacion hombro;
    Articulacion codo;
    Articulacion muneca;
    Articulacion pinza;
    MotorPaso motorPaso;
    SensorPeso sensorPeso;

public:
    BrazoRobotico(String _nombre, Adafruit_PWMServoDriver* _pwm, int _motorPasoDirPin, int _motorPasoStepPin, int _sensorPesoDtPin, int _sensorPesoSckPin)
    : nombre(_nombre), 
      camera(_pwm, 0), 
      base(_pwm, 1), 
      hombro(_pwm, 2), 
      codo(_pwm, 3), 
      muneca(_pwm, 4), 
      pinza(_pwm, 5), 
      motorPaso(_motorPasoDirPin, _motorPasoStepPin), 
      sensorPeso(_sensorPesoDtPin, _sensorPesoSckPin) 
    {}

    void inicializar() {
        camera.inicializar();
        base.inicializar();
        hombro.inicializar();
        codo.inicializar();
        muneca.inicializar();
        pinza.inicializar();
        sensorPeso.tarar();
    }

    void moverArticulacion(String articulacion, int angulo) {
        if (articulacion == "camera") camera.mover(angulo);
        else if (articulacion == "base") base.mover(angulo);
        else if (articulacion == "hombro") hombro.mover(angulo);
        else if (articulacion == "codo") codo.mover(angulo);
        else if (articulacion == "muneca") muneca.mover(angulo);
        else if (articulacion == "pinza") pinza.mover(angulo);
    }

    void moverMotorPaso(int pasos) {
        motorPaso.mover(pasos);
    }

    void actualizar() {
        camera.actualizar();
        base.actualizar();
        hombro.actualizar();
        codo.actualizar();
        muneca.actualizar();
        pinza.actualizar();
        motorPaso.actualizar();
    }

    float obtenerPeso() {
        return sensorPeso.obtenerPeso();
    }

    String getNombre() {
        return nombre;
    }
};
const char* ssid = "RED_THYF";
const char* password = "agapito0810";

AsyncWebServer server(80);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

BrazoRobotico brazo("Brazo_1", &pwm, DIR_PIN, STEP_PIN, 4, 5);

void connectToWiFi() {
    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, password);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to WiFi");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nFailed to connect to WiFi");
        delay(10000);
        ESP.restart();
    }
}
void setup() {
    Serial.begin(115200);

    connectToWiFi();

    pwm.begin();
    pwm.setPWMFreq(60);

    brazo.inicializar();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("camera") 
        && request->hasParam("base") 
        && request->hasParam("hombro") 
        && request->hasParam("codo") 
        && request->hasParam("muneca") 
        && request->hasParam("pinza")
        && request->hasParam("motorPaso")) {

            int cameraVal = request->getParam("camera")->value().toInt();
            int baseVal = request->getParam("base")->value().toInt();
            int hombroVal = request->getParam("hombro")->value().toInt();
            int codoVal = request->getParam("codo")->value().toInt();
            int munecaVal = request->getParam("muneca")->value().toInt();
            int pinzaVal = request->getParam("pinza")->value().toInt();
            int motorPasoVal = request->getParam("motorPaso")->value().toInt();

            Serial.printf("Camera: %d, Base: %d, Hombro: %d, Codo: %d, Muneca: %d, Pinza: %d, MotorPaso: %d\n",
                          cameraVal, baseVal, hombroVal, codoVal, munecaVal, pinzaVal, motorPasoVal);

            brazo.moverArticulacion("camera", cameraVal);
            brazo.moverArticulacion("base", baseVal);
            brazo.moverArticulacion("hombro", hombroVal);
            brazo.moverArticulacion("codo", codoVal);
            brazo.moverArticulacion("muneca", munecaVal);
            brazo.moverArticulacion("pinza", pinzaVal);
            brazo.moverMotorPaso(motorPasoVal);

            request->send(200, "text/plain", "Servomotores y motor paso a paso movidos");
        } else {
            request->send(400, "text/plain", "Faltan parámetros");
        }
    });

    server.begin();
}

void loop() {
    brazo.actualizar();
    float peso = brazo.obtenerPeso();
    Serial.printf("Peso actual: %.2f g\n", peso);
    delay(1000);
}
