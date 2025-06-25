// https://wokwi.com/projects/434497090133072897

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Cables
#define CABLE_0 3
#define CABLE_1 4
#define CABLE_2 5
#define CABLE_3 6

// LED RGB
#define LED_R 9
#define LED_G 10
#define LED_B 11

// aliasLCD
LiquidCrystal_I2C digitalLCD(0x27, 16, 2);

// Variables
long valorRandom;
bool cableCortado[4] = {false, false, false, false};
int pasoActual = 0;

// Casos posibles - orden esperado de los cables
int ordenCorrecto[4][4] = {
  {CABLE_0, CABLE_1, CABLE_2, CABLE_3}, // codigo 0
  {CABLE_1, CABLE_3, CABLE_0, CABLE_2}, // codigo 1
  {CABLE_3, CABLE_0, CABLE_2, CABLE_1}, // codigo 2
  {CABLE_2, CABLE_0, CABLE_1, CABLE_3}  // codigo 3
};


void setup() {
  // Inicializo el LCD
  digitalLCD.init();
  digitalLCD.init();
  digitalLCD.backlight();
  digitalLCD.clear();

  digitalLCD.setCursor(0,0);
  digitalLCD.print("Armada");

  // Configuracion de pines
  pinMode(CABLE_0, INPUT_PULLUP);
  pinMode(CABLE_1, INPUT_PULLUP);
  pinMode(CABLE_2, INPUT_PULLUP);
  pinMode(CABLE_3, INPUT_PULLUP);

  // LED RGB como salida
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  // https://docs.arduino.cc/language-reference/en/functions/random-numbers/random/
  randomSeed(analogRead(0)); // Si el pin analogico 0 esta desconectado, el ruido hace que randomSeed() genere numeros random
  valorRandom = random(4); // Valor random entre 0 y 3
  
  // Muestro el valor random en el LCD
  digitalLCD.setCursor(0,1);
  digitalLCD.print("El codigo es: ");
  digitalLCD.setCursor(14,1);
  digitalLCD.print(valorRandom);

  // Luz azul indica que esta a la espera del usuario
  luzAzul();
}


void loop() {
  // Cambio de dificultad, relevante en el proyecto principal
  int cantidadCables = 4; // Maximo 4, minimo 1
  
  // Se elige un codigo al azar y se toma en cuenta los pasos a seguir
  int cableEsperado = ordenCorrecto[valorRandom][pasoActual];

  // Cable correcto
  if (digitalRead(cableEsperado) == HIGH) {
    cableCortado[pasoActual] = true;
    pasoActual++; // Actualizo el contador
    
    // Luz verde indica corte correcto
    luzVerde();
    
    // Checkea si el usuario gano
    if (pasoActual == cantidadCables) {
      digitalLCD.clear();
      digitalLCD.setCursor(0,0);
      digitalLCD.print("Desactivada!");
      
      while (true) {luzVerde();} // Ganaste!
    }

    delay(500); // TODO: Cambiar por millis?
    
    luzAzul();
  }

  // Reviso si se corto un cable fuera de orden
  for (int i = 0; i < cantidadCables; i++) {
    // Saco el pin del cable que deberia estar en la posición i
    int pin = ordenCorrecto[valorRandom][i];
    
    // Si NO es el paso actual, todavia no se corto y ahora esta cortado = error
    if (i != pasoActual && !cableCortado[i] && digitalRead(pin) == HIGH) {
    	digitalLCD.clear();
      digitalLCD.setCursor(0,0);
      digitalLCD.print("F"); // Mensaje cuando explota
  		
  		while (true) {luzRojaRespirando(); } // F en el chat
    }
  }
}

// Funciones
void luzRoja(){
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);
}

void luzVerde(){
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_B, LOW);
}

void luzAzul(){
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, HIGH);
}

void luzRojaRespirando() {
  for (int i = 0; i < 256; i++) {
    analogWrite(LED_R, i);     // Aumenta la intensidad
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, LOW);
    delay(5);                  // Controla la velocidad del encendido
  }
  for (int i = 255; i >= 0; i--) {
    analogWrite(LED_R, i);     // Disminuye la intensidad
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, LOW);
    delay(5);                  // Controla la velocidad del apagado
  }
}