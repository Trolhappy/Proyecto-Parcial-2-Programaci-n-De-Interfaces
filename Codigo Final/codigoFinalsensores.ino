// Pines para los sensores ultrasónicos
const int TRIG_PUERTA = 6;
const int ECHO_PUERTA = 9;
const int TRIG_POSTE = 7;
const int ECHO_POSTE = 10;
const int TRIG_SALA = 6;
const int ECHO_SALA = 9;
const int TRIG_COCINA = 6;
const int ECHO_COCINA = 9;

// LEDs que se activan con los sensores
const int LED_PUERTA = 3;
const int LED_POSTE= 4;
const int LED_SALA = 2;
const int LED_COCINA = 5;

// Estados de activación de los sensores
bool puerta_activo = true;
bool poste_activo = true;
bool sala_activo = true;
bool cocina_activo = true;

// Estados de los LEDs (para mantener encendidos)
bool luz_puerta_encendida = false;
bool luz_poste_encendida = false;
bool luz_sala_encendida = false;
bool luz_cocina_encendida = false;

// Umbral de distancia para activar el LED (en cm)
const int DISTANCE_THRESHOLD = 40;

void setup() {
  Serial.begin(9600);
  
  // Configurar sensores ultrasónicos
  pinMode(TRIG_PUERTA, OUTPUT);
  pinMode(ECHO_PUERTA, INPUT);
  pinMode(TRIG_POSTE, OUTPUT);
  pinMode(ECHO_POSTE, INPUT);
  pinMode(TRIG_SALA, OUTPUT);
  pinMode(ECHO_SALA, INPUT);
  pinMode(TRIG_COCINA , OUTPUT);
  pinMode(ECHO_COCINA , INPUT);

  // Configurar LEDs
  pinMode(LED_PUERTA, OUTPUT);
  pinMode(LED_POSTE, OUTPUT);
  pinMode(LED_SALA, OUTPUT);
  pinMode(LED_COCINA, OUTPUT);
}

void loop() {
  // Leer comandos de la PC
  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n');
    if (comando == "puerta:ON") puerta_activo = true;
    if (comando == "puerta:OFF") puerta_activo = false;
    if (comando == "poste:ON") poste_activo = true;
    if (comando == "poste:OFF") poste_activo = false;
    if (comando == "sala:ON") sala_activo = true;
    if (comando == "sala:OFF") sala_activo = false;
    if (comando == "cocina:ON") cocina_activo = true;
    if (comando == "cocina:OFF") cocina_activo = false;

    // También permitir apagar las luces manualmente
    if (comando == "luz_puerta:OFF") luz_puerta_encendida = false;
    if (comando == "luz_poste:OFF") luz_poste_encendida = false;
    if (comando == "luz_sala:OFF") luz_sala_encendida = false;
    if (comando == "luz_cocina:OFF") luz_cocina_encendida = false;
    if (comando == "luz_puerta:ON") luz_puerta_encendida = true;
    if (comando == "luz_poste:ON") luz_poste_encendida = true;
    if (comando == "luz_sala:ON") luz_sala_encendida = true;
    if (comando == "luz_cocina:ON") luz_cocina_encendida = true;
  }

  // Verificar sensores SOLO si están activos
  if (puerta_activo && medirDistancia(TRIG_PUERTA, ECHO_PUERTA) < DISTANCE_THRESHOLD)
    luz_puerta_encendida = true;
    
  if (poste_activo && medirDistancia(TRIG_POSTE, ECHO_POSTE) < DISTANCE_THRESHOLD)
    luz_poste_encendida = true;
    
  if (sala_activo && medirDistancia(TRIG_SALA, ECHO_SALA) < DISTANCE_THRESHOLD)
    luz_sala_encendida = true;
    
  if (cocina_activo && medirDistancia(TRIG_COCINA, ECHO_COCINA) < DISTANCE_THRESHOLD)
    luz_cocina_encendida = true;

  // Control de LEDs basado en los estados de encendido
  digitalWrite(LED_PUERTA, luz_puerta_encendida ? HIGH : LOW);
  digitalWrite(LED_POSTE, luz_poste_encendida ? HIGH : LOW);
  digitalWrite(LED_SALA, luz_sala_encendida ? HIGH : LOW);
  digitalWrite(LED_COCINA, luz_cocina_encendida ? HIGH : LOW);

  delay(100); // Pequeño retardo para estabilidad
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