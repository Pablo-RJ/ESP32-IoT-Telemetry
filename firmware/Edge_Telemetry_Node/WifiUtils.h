// WifiUtils.h
#ifndef WIFI_UTILS_H
#define WIFI_UTILS_H

#include <WiFi.h>
#include <Arduino.h>

void conectar_wifi_e_imprimir_ip(const char* ssid, const char* password) {
  delay(10);
  Serial.println(); 
  Serial.print("Conectando a "); 
  Serial.println(ssid); 

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado!");
  Serial.print("Dirección IP del ESP32: ");
  Serial.println(WiFi.localIP());
}

#endif // WIFI_UTILS_H