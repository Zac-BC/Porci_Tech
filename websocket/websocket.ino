#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

const char* ssid = "TU_SSID";
const char* password = "TU_PASSWORD";

WebServer webServer(80);
WebSocketsServer webSocketServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Desconectado!\n", num);
      break;
    case WStype_CONNECTED: {
      IPAddress ip = webSocketServer.remoteIP(num);
      Serial.printf("[%u] Conectado desde %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      // enviar mensaje al cliente recién conectado
      DynamicJsonDocument doc(128);
      doc["mensaje"] = "¡Hola desde el ESP32!";
      String jsonString;
      serializeJson(doc, jsonString);
      webSocketServer.sendTXT(num, jsonString);
    }
    break;
    case WStype_TEXT:
      Serial.printf("[%u] Recibido Texto: %s\n", num, payload);
      // Procesar el mensaje JSON recibido
      DynamicJsonDocument doc(128);
      deserializeJson(doc, payload);
      String comando = doc["comando"].as<String>();
      if (comando == "leerSensor") {
        // Simular lectura de sensor
        DynamicJsonDocument responseDoc(128);
        responseDoc["temperatura"] = random(20, 30);
        String responseJson;
        serializeJson(responseDoc, responseJson);
        webSocketServer.sendTXT(num, responseJson);
      } else if (comando == "controlLed") {
        int estado = doc["estado"].as<int>();
        digitalWrite(LED_BUILTIN, estado);
      }
      break;
    case WStype_BIN:
      Serial.printf("[%u] Recibido Binario (%u bytes)\n", num, length);
      // Puedes manejar datos binarios si es necesario
      // hexdump(payload, length);
      break;
    case WStype_PONG:
      Serial.printf("[%u] Recibido Pong!\n", num);
      break;
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("WiFi conectado");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  webServer.begin();
  webSocketServer.begin();
  webSocketServer.onEvent(webSocketEvent);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  webSocketServer.loop();
  delay(1);
}