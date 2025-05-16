#include <BluetoothSerial.h>  

BluetoothSerial SerialBT;  

String device_name = "ESP32-BT-Slave";  

// Verificación de Bluetooth
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

// Puertos del ESP32
const int PuertoTemperatura = 33;
const int PuertoHumedad = 32;
const int PuertoPH = 35;

// Variables de almacenamiento
int temperatura = 0;
int humedad = 0;
int ph = 0;

unsigned long previousMillis = 0;  
const long interval = 1000;  // Enviar datos cada 1 segundo

void setup() {
    Serial.begin(115200);  
    Serial.println("Conectando bluetooth...");
    SerialBT.begin(device_name);  

    Serial.printf("Se ha iniciado el dispositivo con el nombre \"%s\". \n¡Ahora puedes emparejarlo con Bluetooth!\n", device_name.c_str());
}

void loop() {
    unsigned long currentMillis = millis();

    // Enviar mensaje JSON recurrentemente cada 1 segundo
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        // Leer valores de los sensores
        temperatura = analogRead(PuertoTemperatura);
        humedad = analogRead(PuertoHumedad);
        ph = analogRead(PuertoPH);

        // Normalización de valores
        float temperaturaReal = (temperatura / 4095.0) * 100.0;
        float humedadReal = (humedad / 4095.0) * 100.0;
        float phReal = (ph / 4095.0) * 14.0;

        // Crear mensaje JSON con los datos
        String mensaje = "{";
        mensaje += "\"Temperatura\":" + String(temperaturaReal) + ",";
        mensaje += "\"Humedad\":" + String(humedadReal) + ",";
        mensaje += "\"PH\":" + String(phReal);
        mensaje += "}";

        // Enviar datos por Bluetooth
        SerialBT.println(mensaje);
        Serial.println("Mensaje enviado por Bluetooth: " + mensaje);
    }

    // Comunicación estándar por Bluetooth
    if (Serial.available()) {
        SerialBT.write(Serial.read());
    }
    if (SerialBT.available()) {
        Serial.write(SerialBT.read());
    }

    delay(10);
}