// Define los pines para el sensor
const int trigPin = 5;
const int echoPin = 4;

// Variables para almacenar la duración del pulso y la distancia
long duracion;
int distancia;

void setup() {
  // Inicializa la comunicación serial para mostrar los resultados
  Serial.begin(9600);
  // Configura los pines como salida (Trig) y entrada (Echo)
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Limpia el pin Trig poniéndolo en LOW por un corto tiempo
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Envía un pulso de 10 microsegundos al pin Trig
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Mide la duración del pulso en el pin Echo
  duracion = pulseIn(echoPin, HIGH);

  // Calcula la distancia en centímetros (la velocidad del sonido es aproximadamente 343 m/s o 0.0343 cm/microsegundo)
  // Dividimos entre 2 porque el sonido viaja hasta el objeto y regresa
  distancia = duracion * 0.0343 / 2;

  // Muestra la distancia en el monitor serial
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  // Espera un poco antes de la siguiente medición
  delay(100);
}