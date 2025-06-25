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
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  // https://docs.arduino.cc/language-reference/en/functions/random-numbers/random/
  randomSeed(analogRead(0)); // Si el pin analógico 0 está desconectado, el ruido hace que randomSeed() genere números random
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
  // Se elige un modo al azar y se toma en cuenta los pasos a seguir
  int pinEsperado = ordenCorrecto[valorRandom][pasoActual];

  // Cable correcto
  if (digitalRead(pinEsperado) == HIGH) {
    cableCortado[pasoActual] = true;
    pasoActual++; // Actualizo el contador
    
    // Luz verde indica corte correcto
    luzVerde();
    
    // Checkea si el usuario gano
    if (pasoActual == 4) {
      digitalLCD.clear();
      digitalLCD.setCursor(0,0);
      digitalLCD.print("Desactivada!");
      
      luzVerde(); // Ganaste!
      while (true) {/* nada */}
    }

    delay(500); // TODO: Cambiar por millis?
    
    luzAzul();
  }

  // Reviso si se corto un cable fuera de orden
  for (int i = 0; i < 4; i++) {
    int pin = ordenCorrecto[valorRandom][i];
    if (i != pasoActual && !cableCortado[i] && digitalRead(pin) == HIGH) {
    	digitalLCD.clear();
      digitalLCD.setCursor(0,0);
      digitalLCD.print("nt"); // Mensaje cuando explota
  		
      luzRoja(); // Indica boom
  		while (true) {/* nada */}
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