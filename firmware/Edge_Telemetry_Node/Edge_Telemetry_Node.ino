#include <WiFi.h>
#include <PubSubClient.h>
#include "WifiUtils.h"
#include "DHT.h" 
#include "SecretsExample.h"

// --- 1. CONFIGURACIÓN DEL SENSOR DHT11 ---
#define DHTPIN 4      // Pin digital del ESP32 donde está conectado el sensor DHT
#define DHTTYPE DHT11 // Tipo de sensor: DHT11 (o usa DHT22 si es el caso)

// --- 1. CONFIGURACIÓN DE LA RED WIFI (Usando secrets.h) ---
const char* ssid = SECRET_SSID;         
const char* password = SECRET_PASS;        

// --- 2. CONFIGURACIÓN DEL BROKER MQTT (Usando secrets.h) ---
const char* mqtt_server = SECRET_MQTT_SERVER;         
const int mqtt_port = SECRET_MQTT_PORT;                       
const char* mqtt_client_id = "ESP32_Edge_Node_v1";   

// -- TOPICOS --
const char* mqtt_topic_temp = "esp/datos/temperatura"; 

// Objetos requeridos
WiFiClient espClient;            
PubSubClient client(espClient);  
long lastMsg = 0;                
char msg[150];                 

// Inicializa el objeto DHT
DHT dht(DHTPIN, DHTTYPE);

// =========================================================
// FUNCIONES DE SOPORTE
// =========================================================

// Intenta reconectar el cliente MQTT al broker.
void reconnect() {
  // Bucle hasta que estemos reconectados
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    // Intenta conectar usando el ID del cliente
    if (client.connect(mqtt_client_id)) {
      Serial.println("Conectado!");
    } else {
      Serial.print("falló, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

// =========================================================
// SETUP Y LOOP
// =========================================================

void setup() {
  // Inicia la comunicación serial para depuración y monitoreo
  Serial.begin(115200);
  // Conecta a Wi-Fi (esta función incluye la impresión de la IP local del ESP32)
  conectar_wifi_e_imprimir_ip(ssid, password);
  // Configura la dirección IP y el puerto del Broker MQTT para la librería  
  client.setServer(mqtt_server, mqtt_port);
  
  // AÑADIDO: Inicializa el sensor DHT
  dht.begin();
  Serial.println("Sensor DHT inicializado.");
}


void loop() {
  // Asegúrate de que el cliente esté conectado antes de publicar
  if (!client.connected()) {
    reconnect();
  }
  
  client.loop(); // Permite al cliente MQTT mantener su conexión
  long now = millis();

  // Publicar cada 5 segundos (5000 milisegundos)
  if (now - lastMsg > 2000) {
    lastMsg = now;

    // 1. Leer el sensor
    float t = dht.readTemperature();
    float h = dht.readHumidity(); // La humedad es un buen dato para incluir

    // 2. Comprobar si la lectura es válida
    if (isnan(t) || isnan(h)) {
      Serial.println("¡Error! Falló la lectura del sensor DHT.");
      return; // Salimos sin publicar
    }

    // 3. Crear el mensaje JSON (Fácil de analizar para tu servidor)
    // %.2f asegura 2 decimales para la temperatura
    snprintf (msg, 150, 
                "{\"dispositivo\":\"%s\",\"temperatura\":%.2f,\"humedad\":%.0f,\"timestamp\":%ld}", 
                mqtt_client_id, t, h, now / 1000);

    Serial.print("Publicando JSON: ");
    Serial.println(msg);

    // 4. Publicar en el tópico configurado
    client.publish(mqtt_topic_temp, msg);
  }
}