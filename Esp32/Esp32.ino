#include "BluetoothSerial.h"

String device_name = "ESP32-BT-Slave";


void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  if (type == WStype_TEXT) {
    // Lógica mínima sin Serial
  }
}

void setup() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  webSocket.begin(server, port, "/");
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
} 
