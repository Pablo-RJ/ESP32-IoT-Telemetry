#include <WiFi.h>
#include <PubSubClient.h>
#include "WifiUtils.h"
#include "DHT.h" 
#include "SecretsExample.h"

// --- 1. CONFIGURACIÓN DEL SENSOR DHT11 ---
#define DHTPIN 4      // Pin digital del ESP32 donde está conectado el sensor DHT
#define DHTTYPE DHT11 // Tipo de sensor: DHT11 (o usa DHT22 si es el caso)
DHT dht(DHTPIN, DHTTYPE);

// --- 2. CONFIGURACIÓN DE LA RED WIFI (Usando secrets.h) ---
const char* ssid = SECRET_SSID;         
const char* password = SECRET_PASS;        
// --- 3. CONFIGURACIÓN DEL BROKER MQTT (Usando secrets.h) ---
const char* mqtt_server = SECRET_MQTT_SERVER;         
const int mqtt_port = SECRET_MQTT_PORT;                       
const char* mqtt_client_id = "ESP32_DeepSleep_Node";   
// --- 4. CONFIGURACION DE DEEP SLEEP ---
#define uS_TO_S_FACTOR 1000000ULL  /* Factor de conversión de microsegundos a segundos */
#define TIME_TO_SLEEP  10          /* Tiempo que dormirá (en segundos) */
// -- TOPICOS --
const char* mqtt_topic_temp = "esp/datos/temperatura"; 

// Objetos
WiFiClient espClient;
PubSubClient client(espClient);
char msg[150];

void reconnect() {
  // Loop simple para reconectar si falla (bloqueante)
  while (!client.connected()) {
    Serial.print("Conectando a MQTT...");
    if (client.connect(mqtt_client_id)) {
      Serial.println("Conectado!");
    } else {
      Serial.print("falló, rc=");
      Serial.print(client.state());
      Serial.println(" reintentando en 2s...");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000); // Pequeña espera para que el monitor serie arranque

  // 1. Iniciar Sensor
  dht.begin();
  Serial.println("Despertando... Sensor iniciado.");

  // 2. Conectar WiFi (Usamos tu librería auxiliar)
  conectar_wifi_e_imprimir_ip(ssid, password);

  // 3. Conectar MQTT
  client.setServer(mqtt_server, mqtt_port);
  if (!client.connected()) {
    reconnect();
  }

  // 4. Leer datos
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // Verificar lecturas
  if (isnan(t) || isnan(h)) {
    Serial.println("Error leyendo el sensor DHT");
    // Incluso si falla, enviamos error o simplemente dormimos
  } else {
    // 5. Enviar Datos
    snprintf(msg, 150, 
            "{\"dispositivo\":\"%s\",\"temperatura\":%.2f,\"humedad\":%.0f,\"modo\":\"deep_sleep\"}", 
            mqtt_client_id, t, h);
            
    Serial.print("Publicando: ");
    Serial.println(msg);
    client.publish(mqtt_topic_temp, msg);
    
    // IMPORTANTE: Dar tiempo al buffer de red para enviar antes de cortar la energía
    delay(500); 
  }

  // 6. Preparar el sueño
  Serial.println("Entrando en Deep Sleep...");
  client.disconnect(); // Desconexión limpia MQTT
  
  // Configurar el temporizador de despertar
  esp_deep_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  
  // Iniciar Deep Sleep (Aquí se apaga todo)
  esp_deep_sleep_start();
}

void loop() {
  // En Deep Sleep, el loop NUNCA se ejecuta.
}