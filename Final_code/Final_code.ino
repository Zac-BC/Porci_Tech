#include "BluetoothSerial.h"
#include "ESP32Servo.h"

Servo miServo;
Servo miServo2;
String device_name = "ZacBC_ESP32";

// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to and enable it
#endif

// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

// Puertos del ESP32
const int PuertoTemperatura = 33;
const int PuertoHumedad = 32;
const int PuertoPH = 35;
// LED Indicador
const int ledPin = 2;
// Puertos Servos
const int Servo_principal = 13;
const int Servo_secundario = 12;
// Puerto Ultrasonico
const int trigPin = 5;
const int echoPin = 4;

// Variables de almacenamiento
int temperatura = 0;
int humedad = 0;
int ph = 0;
long duracion;
int distancia;


void setup() {
    // LED indicador
    pinMode(ledPin, OUTPUT);
    // Baudios de la comunicación serial
    Serial.begin(115200);
    //Configuracion Servos
    miServo2.attach(Servo_secundario);
    miServo2.write(0);
    miServo.attach(Servo_principal);
    miServo.write(90);
    // Configura los pines como salida (Trig) y entrada (Echo)
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    //Configuracion de bluetooth
    SerialBT.begin(device_name);
    Serial.println("Puedes conectarte a: " + device_name);
    // Espera hasta que se conecte al Dispositibo bluetooth
    Serial.println("Esperando conexión Bluetooth...");
    while (!SerialBT.connected()) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nDispositivo Bluetooth conectado!");
}

void loop() {
    // Limpia el pin Trig poniéndolo en LOW por un corto tiempo
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);


    // Leer valores de los sensores
    temperatura = analogRead(PuertoTemperatura);
    humedad = analogRead(PuertoHumedad);
    ph = analogRead(PuertoPH);

    
    // Envía un pulso de 10 microsegundos al pin Trig
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Mide la duración del pulso en el pin Echo
    duracion = pulseIn(echoPin, HIGH);

    // Calcula la distancia en centímetros (la velocidad del sonido es aproximadamente 343 m/s o 0.0343 cm/microsegundo)
    // Dividimos entre 2 porque el sonido viaja hasta el objeto y regresa
    distancia = duracion * 0.0343 / 2;

    // Normalización de valores (puedes ajustarlo según tu calibración)
    float temperaturaReal = (temperatura / 4095.0) * 100.0;
    float humedadReal = (humedad / 4095.0) * 100.0;
    float phReal = (ph / 4095.0) * 14.0;

    // Crear un mensaje JSON con los datos
    String mensaje = "{";
    mensaje += "\"Temperatura\":" + String(temperaturaReal) + ",";
    mensaje += "\"Humedad\":" + String(humedadReal) + ",";
    mensaje += "\"PH\":" + String(phReal);
    mensaje += "\"Distancia\":"+ String(distancia);
    mensaje += "}";

    // Enviar el mensaje JSON a través de Bluetooth en cada ciclo
    Serial.println(mensaje);
    SerialBT.write((uint8_t*)mensaje.c_str(), mensaje.length());


    // Pasar datos desde Bluetooth al monitor serial
    if (SerialBT.available()) {
        char Entrada = SerialBT.read(); // Declaración de la variable 'Entrada' como char
        Serial.write(Entrada);
        if (Entrada == 'H'){
            digitalWrite(ledPin, HIGH);
            miServo.write(0);
            miServo2.write(90);
        } else if(Entrada == 'L'){
            digitalWrite(ledPin, LOW);
            miServo.write(90);
            miServo2.write(0);
        }
    }
    if (Serial.available()) {
        SerialBT.write(Serial.read());
    }
    delay(100); // Pequeña pausa para no saturar
}

