// Programa para ESP32-C3 Mini que controla 2 LEDs de forma rítmica
// LEDs conectados en los pines 2 y 3

const int ledPin1 = 2;  // Pin del primer LED
const int ledPin2 = 3;  // Pin del segundo LED

// Variables para el ritmo
unsigned long previousMillis = 0;
const long interval = 500;  // Intervalo entre cambios en milisegundos
bool ledState1 = LOW;
bool ledState2 = HIGH;

void setup() {
  // Configurar pines como salida
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  
  // Estado inicial
  digitalWrite(ledPin1, ledState1);
  digitalWrite(ledPin2, ledState2);
}

void loop() {
  unsigned long currentMillis = millis();

  // Verificar si ha pasado el intervalo de tiempo
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Cambiar estados de los LEDs
    ledState1 = !ledState1;
    ledState2 = !ledState2;
    
    // Aplicar nuevos estados
    digitalWrite(ledPin1, ledState1);
    digitalWrite(ledPin2, ledState2);
  }
}
