#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// Definir los límites del servo
#define SERVOMIN  150 // Min pulso
#define SERVOMAX  600 // Max pulso

// Definición de la clase Articulacion
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

    void detener() {
        enMovimiento = false;
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

// Configuración de pines del motor paso a paso con A4988
#define STEP_PIN 12   // Pin STEP del A4988
#define DIR_PIN 13    // Pin DIR del A4988
#define ENABLE_PIN 14 // Pin ENABLE del A4988 (opcional)

// Configuración de pines del segundo motor paso a paso con A4988
#define STEP_PIN2 27  // Pin STEP del segundo A4988
#define DIR_PIN2 26   // Pin DIR del segundo A4988

// Variables de control del motor
int stepDelay = 1000; // Tiempo de espera entre pasos (en microsegundos)
int stepCount = 0;   // Contador de pasos

// Configuración de red WiFi
const char* ssid = "Marco Omar";
const char* password = "1720500139M";

AsyncWebServer server(80);

// Crear objeto para el controlador PCA9685
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Instancia de la clase Articulacion para cada servo
Articulacion camera(&pwm, 0);
Articulacion base(&pwm, 1);
Articulacion hombro(&pwm, 2);
Articulacion codo(&pwm, 3);
Articulacion muneca(&pwm, 4);
Articulacion pinza(&pwm, 5);

bool forward = false;
bool backward = false;

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
    // Intentar reconectar después de un tiempo
    delay(10000);
    ESP.restart(); // Reinicia el ESP32
  }
}

void setup() {
  Serial.begin(115200);

  // Intentar conectar a WiFi
  connectToWiFi();

  // Inicialización de PCA9685
  pwm.begin();
  pwm.setPWMFreq(60);  // Frecuencia para servos

  // Inicialización de servos
  camera.inicializar();
  base.inicializar();
  hombro.inicializar();
  codo.inicializar();
  muneca.inicializar();
  pinza.inicializar();

  // Configuración de pines del motor paso a paso con A4988
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT); // Si usas ENABLE_PIN
  
  // Configuración de pines del segundo motor paso a paso
  pinMode(STEP_PIN2, OUTPUT);
  pinMode(DIR_PIN2, OUTPUT);

  // Habilitar los controladores de los motores
  digitalWrite(ENABLE_PIN, LOW); // O HIGH si tu configuración lo requiere

  // Inicializa los pines en LOW
  digitalWrite(STEP_PIN, LOW);
  digitalWrite(DIR_PIN, LOW);
  digitalWrite(STEP_PIN2, LOW);
  digitalWrite(DIR_PIN2, LOW);

  // Configuración del servidor web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("camera") 
    && request->hasParam("base") 
    && request->hasParam("hombro") 
    && request->hasParam("codo") 
    && request->hasParam("muneca") 
    && request->hasParam("pinza")
    && request->hasParam("forward")
    && request->hasParam("backward")) {

      int cameraVal = request->getParam("camera")->value().toInt();
      int baseVal = request->getParam("base")->value().toInt();
      int hombroVal = request->getParam("hombro")->value().toInt();
      int codoVal = request->getParam("codo")->value().toInt();
      int munecaVal = request->getParam("muneca")->value().toInt();
      int pinzaVal = request->getParam("pinza")->value().toInt();
      forward = request->getParam("forward")->value().toInt();
      backward = request->getParam("backward")->value().toInt();

      // Imprimir valores para depuración
      Serial.printf("Camera: %d, Base: %d, Hombro: %d, Codo: %d, Muneca: %d, Pinza: %d\n",
                    cameraVal, baseVal, hombroVal, codoVal, munecaVal, pinzaVal);

      // Mover servos
      camera.mover(cameraVal);
      base.mover(baseVal);
      hombro.mover(hombroVal);
      codo.mover(codoVal);
      muneca.mover(munecaVal);
      pinza.mover(pinzaVal);

      request->send(200, "text/plain", "Servomotores movidos");
    } else {
      request->send(400, "text/plain", "Faltan parámetros");
    }
  });

  server.begin();
}

int lastStepCount = 0;

void loop() {
  // Actualizar estado de servos
  camera.actualizar();
  base.actualizar();
  hombro.actualizar();
  codo.actualizar();
  muneca.actualizar();
  pinza.actualizar();

  // Control de motores paso a paso
  if (forward) {
    stepMotor(STEP_PIN, DIR_PIN, true); // Mueve ambos motores hacia adelante
    stepMotor(STEP_PIN2, DIR_PIN2, true);
  } else if (backward) {
    stepMotor(STEP_PIN, DIR_PIN, false); // Mueve ambos motores hacia atrás
    stepMotor(STEP_PIN2, DIR_PIN2, false);
  }

  // Solo mostrar los pasos si el botón está presionado
  if (forward || backward) {
    if (stepCount != lastStepCount) {
      Serial.printf("Pasos: %d\n", stepCount);
      lastStepCount = stepCount;
    }
  } else {
    // Si ningún botón está presionado, reiniciar el contador de pasos
    lastStepCount = stepCount;
  }

  // Agregar una pequeña demora para evitar el sobrecalentamiento del motor
  delay(10);
}

void stepMotor(int stepPin, int dirPin, bool direction) {
  // Controlar la dirección del motor
  digitalWrite(dirPin, direction ? HIGH : LOW);

  // Realizar un paso del motor
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(stepDelay);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(stepDelay#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// Definir los límites del servo
#define SERVOMIN  150 // Min pulso
#define SERVOMAX  600 // Max pulso

// Definición de la clase Articulacion
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

// Configuración de red WiFi
const char* ssid = "Marco Omar";
const char* password = "1720500139M";

AsyncWebServer server(80);

// Crear objeto para el controlador PCA9685
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Instancia de la clase Articulacion para cada servo
Articulacion camera(&pwm, 0);
Articulacion base(&pwm, 1);
Articulacion hombro(&pwm, 2);
Articulacion codo(&pwm, 3);
Articulacion muneca(&pwm, 4);
Articulacion pinza(&pwm, 5);

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
    // Intentar reconectar después de un tiempo
    delay(10000);
    ESP.restart(); // Reinicia el ESP32
  }
}

void setup() {
  Serial.begin(115200);

  // Intentar conectar a WiFi
  connectToWiFi();

  // Inicialización de PCA9685
  pwm.begin();
  pwm.setPWMFreq(60);  // Frecuencia para servos

  // Inicialización de servos
  camera.inicializar();
  base.inicializar();
  hombro.inicializar();
  codo.inicializar();
  muneca.inicializar();
  pinza.inicializar();

  // Configuración del servidor web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("camera") 
    && request->hasParam("base") 
    && request->hasParam("hombro") 
    && request->hasParam("codo") 
    && request->hasParam("muneca") 
    && request->hasParam("pinza")
    && request->hasParam("forward")
    && request->hasParam("backward")
    && request->hasParam("left")
    && request->hasParam("right")) {

      int cameraVal = request->getParam("camera")->value().toInt();
      int baseVal = request->getParam("base")->value().toInt();
      int hombroVal = request->getParam("hombro")->value().toInt();
      int codoVal = request->getParam("codo")->value().toInt();
      int munecaVal = request->getParam("muneca")->value().toInt();
      int pinzaVal = request->getParam("pinza")->value().toInt();
      int forwardVal = request->getParam("forward")->value().toInt();
      int backwardVal = request->getParam("backward")->value().toInt();
      int leftVal = request->getParam("left")->value().toInt();
      int rightVal = request->getParam("right")->value().toInt();

      // Imprimir valores para depuración
      Serial.printf("Camera: %d, Base: %d, Hombro: %d, Codo: %d, Muneca: %d, Pinza: %d\n",
                    cameraVal, baseVal, hombroVal, codoVal, munecaVal, pinzaVal);

      // Mover servos
      camera.mover(cameraVal);
      base.mover(baseVal);
      hombro.mover(hombroVal);
      codo.mover(codoVal);
      muneca.mover(munecaVal);
      pinza.mover(pinzaVal);

      request->send(200, "text/plain", "Servomotores movidos");
    } else {
      request->send(400, "text/plain", "Faltan parámetros");
    }
  });

  server.begin();
}

void loop() {
  // Actualizar estado de servos
  camera.actualizar();
  base.actualizar();
  hombro.actualizar();
  codo.actualizar();
  muneca.actualizar();
  pinza.actualizar();
});

  // Incrementar el contador de pasos
  stepCount++;
}
