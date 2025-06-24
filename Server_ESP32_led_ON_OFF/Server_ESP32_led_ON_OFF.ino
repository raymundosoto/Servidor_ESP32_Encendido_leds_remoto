#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Configuración de red
const char* ssid = "laptop_ray";
const char* password = "44667788";

// Configuración del servidor Flask
const char* flaskServer = "raymundoss.pythonanywhere.com";
const String endpoint = "/update_status";

// Pines de los LEDs
const int ledPin1 = 2;  // Cambia según tu configuración
const int ledPin2 = 3;  // Cambia según tu configuración

// Identificador del equipo
const String deviceId = "Unitec_Ecatepec_02";

WebServer server(80);

// Variables para el estado de los LEDs
bool led1Status = false;
bool led2Status = false;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  
  // Conectar a WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Configurar rutas del servidor web
  server.on("/", handleRoot);
  server.on("/led1on", handleLed1On);
  server.on("/led1off", handleLed1Off);
  server.on("/led2on", handleLed2On);
  server.on("/led2off", handleLed2Off);
  
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = R"=====(
  <!DOCTYPE html>
  <html lang="es">
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Control de LEDs - Unitec Ecatepec</title>
    <style>
      body {
        font-family: Arial, sans-serif;
        text-align: center;
        background-color: #f0f0f0;
        margin: 0;
        padding: 20px;
      }
      h1 {
        color: #003366;
      }
      .btn {
        background-color: #0066cc;
        color: white;
        border: none;
        padding: 10px 20px;
        margin: 10px;
        border-radius: 5px;
        cursor: pointer;
        font-size: 16px;
        transition: background-color 0.3s;
      }
      .btn:hover {
        background-color: #004d99;
      }
      .btn-off {
        background-color: #cc0000;
      }
      .btn-off:hover {
        background-color: #990000;
      }
      .status {
        font-weight: bold;
        margin: 20px;
      }
    </style>
  </head>
  <body>
    <h1>Unitec Ecatepec - Skill Education 2025</h1>
    <h2>Control de LEDs</h2>
    
    <div>
      <h3>LED 1</h3>
      <button class="btn" onclick="location.href='/led1on'">Encender</button>
      <button class="btn btn-off" onclick="location.href='/led1off'">Apagar</button>
      <p class="status">Estado: %LED1STATUS%</p>
    </div>
    
    <div>
      <h3>LED 2</h3>
      <button class="btn" onclick="location.href='/led2on'">Encender</button>
      <button class="btn btn-off" onclick="location.href='/led2off'">Apagar</button>
      <p class="status">Estado: %LED2STATUS%</p>
    </div>
  </body>
  </html>
  )=====";

  // Reemplazar marcadores de posición con el estado actual
  html.replace("%LED1STATUS%", led1Status ? "Encendido" : "Apagado");
  html.replace("%LED2STATUS%", led2Status ? "Encendido" : "Apagado");
  
  server.send(200, "text/html", html);
}

void handleLed1On() {
  digitalWrite(ledPin1, HIGH);
  led1Status = true;
  sendStatusToFlask();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleLed1Off() {
  digitalWrite(ledPin1, LOW);
  led1Status = false;
  sendStatusToFlask();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleLed2On() {
  digitalWrite(ledPin2, HIGH);
  led2Status = true;
  sendStatusToFlask();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleLed2Off() {
  digitalWrite(ledPin2, LOW);
  led2Status = false;
  sendStatusToFlask();
  server.sendHeader("Location", "/");
  server.send(303);
}

void sendStatusToFlask() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    // Construir la URL del endpoint
    String url = "http://" + String(flaskServer) + endpoint;
    
    // Crear el objeto JSON
    DynamicJsonDocument doc(200);
    doc["device_id"] = deviceId;
    doc["led1_status"] = led1Status;
    doc["led2_status"] = led2Status;
    
    String jsonString;
    serializeJson(doc, jsonString);
    
    // Configurar la solicitud HTTP
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    
    // Enviar la solicitud POST
    int httpResponseCode = http.POST(jsonString);
    
    if (httpResponseCode > 0) {
      Serial.print("Código de respuesta HTTP: ");
      Serial.println(httpResponseCode);
      String response = http.getString();
      Serial.println(response);
    } else {
      Serial.print("Error en la solicitud: ");
      Serial.println(httpResponseCode);
    }
    
    http.end();
  } else {
    Serial.println("WiFi desconectado");
  }
}
