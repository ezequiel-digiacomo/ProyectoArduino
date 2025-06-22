// https://wokwi.com/projects/434416853604919297

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define CABLE_0 3
#define CABLE_1 4
#define CABLE_2 5
#define CABLE_3 6
#define LED_PIN 13

LiquidCrystal_I2C digitalLCD(0x27, 16, 2);

long valorRandom;
int i = 0;
bool cableCortado[4] = {false, false, false, false};
int ordenCorrecto[4][4] = {
  {CABLE_0, CABLE_1, CABLE_2, CABLE_3}, // caso 0
  {CABLE_1, CABLE_3, CABLE_0, CABLE_2}, // caso 1
  {CABLE_3, CABLE_0, CABLE_2, CABLE_1}, // caso 2
  {CABLE_2, CABLE_0, CABLE_1, CABLE_3}  // caso 3
};
int pasoActual = 0;

void setup() {
  // Inicializo el LCD
  digitalLCD.init();
  digitalLCD.init();
  digitalLCD.backlight();
  digitalLCD.clear();
  
  // Conexion serial
  Serial.begin(9600);
  
  digitalLCD.setCursor(0,0);
  digitalLCD.print("Armada");

  // Configuracion de pines
  pinMode(CABLE_0, INPUT_PULLUP);
  pinMode(CABLE_1, INPUT_PULLUP);
  pinMode(CABLE_2, INPUT_PULLUP);
  pinMode(CABLE_3, INPUT_PULLUP);

  // Si la bomba explota, se prende el led integrado
  pinMode(LED_PIN, OUTPUT);

  // https://docs.arduino.cc/language-reference/en/functions/random-numbers/random/
  randomSeed(analogRead(0)); // Si el pin analógico 0 está desconectado, el ruido hace que randomSeed() genere números random
  valorRandom = random(4); // Valor random entre 0 y 3
  
  // Muestro el valor random en el LCD
  digitalLCD.setCursor(0,1);
  digitalLCD.print("El codigo es: ");
  digitalLCD.setCursor(14,1);
  digitalLCD.print(valorRandom);
  //Serial.print("El codigo es: ");
  //Serial.println(valorRandom);
}

void loop() {

  int pinEsperado = ordenCorrecto[valorRandom][pasoActual];

  if (digitalRead(pinEsperado) == HIGH) {
    Serial.println("Cable correcto cortado");
    cableCortado[pasoActual] = true;
    pasoActual++; // Actualizo el contador

    if (pasoActual == 4) {
      digitalLCD.clear();
      digitalLCD.setCursor(0,0);
      digitalLCD.print("Desactivada!");
      // Serial.println("Bomba desactivada!");
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
      //Serial.println("nt"); // Mensaje cuando explota
  		digitalWrite(LED_PIN, HIGH);
  		while (true) {/* nada */}
    }
  }
}