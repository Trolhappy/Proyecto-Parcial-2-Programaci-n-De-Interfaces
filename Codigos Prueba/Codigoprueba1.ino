const int TRIG_PUERTA = 6;
const int ECHO_PUERTA = 9;
const int TRIG_PATIO = 7;
const int ECHO_PATIO = 10;
const int TRIG_PASILLO = 8;
const int ECHO_PASILLO = 11;

const int LED_PUERTA = 3;
const int LED_PATIO = 4;
const int LED_PASILLO = 5;

//Estados de activación de los sensores
bool puerta_activo = true;
bool patio_activo = true;
bool pasillo_activo = true;

//Umbral de distancia para activar el LED (en cm)
const int DISTANCE_THRESHOLD = 200;

void setup() {
  Serial.begin(9600);
  
  // Configurar sensores ultrasónicos
  pinMode(TRIG_PUERTA, OUTPUT);
  pinMode(ECHO_PUERTA, INPUT);
  pinMode(TRIG_PATIO, OUTPUT);
  pinMode(ECHO_PATIO, INPUT);
  pinMode(TRIG_PASILLO, OUTPUT);
  pinMode(ECHO_PASILLO, INPUT);

  // Configurar LEDs
  pinMode(LED_PUERTA, OUTPUT);
  pinMode(LED_PATIO, OUTPUT);
  pinMode(LED_PASILLO, OUTPUT);
}

void loop() {
  // Leer comandos de la PC
  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n');
    if (comando == "puerta:ON") puerta_activo = true;
    if (comando == "puerta:OFF") puerta_activo = false;
    if (comando == "patio:ON") patio_activo = true;
    if (comando == "patio:OFF") patio_activo = false;
    if (comando == "pasillo:ON") pasillo_activo = true;
    if (comando == "pasillo:OFF") pasillo_activo = false;
  }

  // Verificar sensores y controlar LEDs
  if (puerta_activo && medirDistancia(TRIG_PUERTA, ECHO_PUERTA) < DISTANCE_THRESHOLD)
    digitalWrite(LED_PUERTA, HIGH);
  else
    digitalWrite(LED_PUERTA, LOW);

  if (patio_activo && medirDistancia(TRIG_PATIO, ECHO_PATIO) < DISTANCE_THRESHOLD)
    digitalWrite(LED_PATIO, HIGH);
  else
    digitalWrite(LED_PATIO, LOW);

  if (pasillo_activo && medirDistancia(TRIG_PASILLO, ECHO_PASILLO) < DISTANCE_THRESHOLD)
    digitalWrite(LED_PASILLO, HIGH);
  else
    digitalWrite(LED_PASILLO, LOW);

  delay(500); // Pequeño retardo
}

// Función para medir la distancia con el sensor ultrasónico
float medirDistancia(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH);
  float distancia = duration * 0.017; // Convertir tiempo a cm

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
  
  return distancia;
}