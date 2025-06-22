// https://wokwi.com/projects/434497090133072897

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Cables
#define CABLE_0 3
#define CABLE_1 4
#define CABLE_2 5
#define CABLE_3 6

// LED RGB
#define redPin 9
#define greenPin 10
#define bluePin 11

// aliasLCD
LiquidCrystal_I2C digitalLCD(0x27, 16, 2);

// Variables
long valorRandom;
bool cableCortado[4] = {false, false, false, false};
int pasoActual = 0;

// Casos posibles
int ordenCorrecto[4][4] = {
  {CABLE_0, CABLE_1, CABLE_2, CABLE_3}, // caso 0
  {CABLE_1, CABLE_3, CABLE_0, CABLE_2}, // caso 1
  {CABLE_3, CABLE_0, CABLE_2, CABLE_1}, // caso 2
  {CABLE_2, CABLE_0, CABLE_1, CABLE_3}  // caso 3
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
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // https://docs.arduino.cc/language-reference/en/functions/random-numbers/random/
  randomSeed(analogRead(0)); // Si el pin analógico 0 está desconectado, el ruido hace que randomSeed() genere números random
  valorRandom = random(4); // Valor random entre 0 y 3
  
  // Muestro el valor random en el LCD
  digitalLCD.setCursor(0,1);
  digitalLCD.print("El codigo es: ");
  digitalLCD.setCursor(14,1);
  digitalLCD.print(valorRandom);

  // Luz azul indica que esta a la espera del usuario
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, HIGH);
}

void loop() {
  int pinEsperado = ordenCorrecto[valorRandom][pasoActual];

  // Cable correcto
  if (digitalRead(pinEsperado) == HIGH) {
    cableCortado[pasoActual] = true;
    pasoActual++; // Actualizo el contador
    
    // Luz verde indica corte correcto
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, LOW);

    delay(500); // TODO: Cambiar por millis

    // Luz azul indica que esta a la espera del usuario
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, HIGH);

    // Checkea si el usuario gano
    if (pasoActual == 4) {
      digitalLCD.clear();
      digitalLCD.setCursor(0,0);
      digitalLCD.print("Desactivada!");
      
      // Luz verde
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH);
      digitalWrite(bluePin, LOW);
      while (true) {/* nada */}
    }
  }

  // Reviso si se corto un cable fuera de orden
  for (int i = 0; i < 4; i++) {
    int pin = ordenCorrecto[valorRandom][i];
    if (i != pasoActual && !cableCortado[i] && digitalRead(pin) == HIGH) {
    	digitalLCD.clear();
      digitalLCD.setCursor(0,0);
      digitalLCD.print("nt"); // Mensaje cuando explota
  		
      // Luz roja
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, LOW);
  		while (true) {/* nada */}
    }
  }
}