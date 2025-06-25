#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// aliasLCD
LiquidCrystal_I2C digitalLCD(0x27, 16, 2);

//------------rgb 1 boton-------------
#define redPin 9
#define greenPin 10
#define bluePin 11
// #define button 13
const long colorDuration = 1000; 

int contadorJuegos = 0;

unsigned long previousMillis = 0;
unsigned long auxPreviousMillis = 0;
int colorIndex = 0;

// Colores RGB: Rojo, Azul, Verde, Amarillo
int colors[4][3] = {
  {255, 0, 0},     // Rojo
  {0, 0, 255},     // Azul
  {0, 255, 0},     // Verde
  {255, 255, 0}    // Amarillo
};
int currentColor[3] = {0,0,0};
int colorWinner[3] = {255,0,0}; //rojo 

bool puzzle1LedRgbStatus = false;
//--------------rgb 1 boton----------------

//--------------simon dice-----------------
const int BUTTONS[4] = {13, 12, 8, 7}; // Rojo, Azul, Verde, Amarillo
const int BUZZER_PIN = 1;

const int tones[4] = {440, 494, 523, 587}; // Tonos por color

enum GameState {
  SHOW_SEQUENCE,
  WAIT_INPUT,
  WAIT_SUCCESS_MELODY,
  PLAY_SUCCESS_MELODY,
  LOSE,
  PAUSE_BEFORE_NEXT
};
GameState gameState = SHOW_SEQUENCE;

const int MAX_SEQUENCE = 100;
int sequence[MAX_SEQUENCE];
int sequenceLength = 0;
int playerIndex = 0;

unsigned long lastActionTime = 0;
const unsigned long lightDuration = 500;
const unsigned long pauseBetweenLights = 300;
const unsigned long debounceDelay = 150;
unsigned long lastButtonTime = 0;

bool showing = false;
int showIndex = 0;

// Manejo de error
bool errorSoundPlaying = false;
unsigned long errorSoundStart = 0;
const unsigned long errorSoundDuration = 600;

// Melodía de éxito
const int successMelody[] = {523, 659, 784};          // DO, MI, SOL
const int successDurations[] = {200, 200, 300};
const int successLength = sizeof(successMelody) / sizeof(successMelody[0]);
int melodyIndex = 0;
unsigned long melodyStartTime = 0;

//--------------simon dice-----------------

#define botonPin 2
#define potenciometroPin A1

unsigned long tiempoTotal;
unsigned long restarTiempo = 0; 
bool estadoBotonMenu = false;
bool juegoFinalizado = false;
int vidas = 3;
int dificultad = 1;
bool juegoIniciado = false;
bool aux = true;

byte corazon[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

/*--------------------------- cortar cables ----------------------------------*/

// Cables
#define CABLE_0 3
#define CABLE_1 4
#define CABLE_2 5
#define CABLE_3 6

// LED RGB
#define redPin 9
#define greenPin 10
#define bluePin 11

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

/*--------------------------- cortar cables ----------------------------------*/

void setup() {
  digitalLCD.init();
  digitalLCD.init();
  digitalLCD.backlight();
  digitalLCD.createChar(0, corazon); 
  
  pinMode(botonPin, INPUT_PULLUP);
  pinMode(potenciometroPin, INPUT);
  //--------------------------rgb 1 boton---------------------------------
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  analogWrite(redPin, colors[colorIndex][0]);
  analogWrite(greenPin, colors[colorIndex][1]);
  analogWrite(bluePin, colors[colorIndex][2]);
  //--------------------------rgb 1 boton---------------------------------
  //----------------------------simon dice----------------------------------
  for (int i = 0; i < 4; i++) pinMode(BUTTONS[i], INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  startNewRound();
  //----------------------------simon dice----------------------------------

  /*--------------------------- cortar cables ----------------------------------*/

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

  // Luz azul indica que esta a la espera del usuario
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, HIGH);

  /*--------------------------- cortar cables ----------------------------------*/
}

void loop() {

  //=============== Iniciamos el Juego =============== 
  if (!juegoIniciado) {
    seleccionarDificultad();
    return;
  }

  //=============== Restar Tiempo =============== 
  unsigned long actualMillis = millis();

  if (!juegoFinalizado) {
    mostrarEstado();

    if (actualMillis - restarTiempo >= 1000 && tiempoTotal > 0) {
      tiempoTotal -= 1000;
      restarTiempo = actualMillis;
      mostrarEstado();
    }

    switch (contadorJuegos)
    {
    case 0:
    /*
    //---------------------------rgb 1 boton----------------------------------
      unsigned long auxCurrentMillis = actualMillis;
      if(aux){
        auxPreviousMillis = auxCurrentMillis;
        aux = false;
      }
      if (auxCurrentMillis - auxPreviousMillis >= colorDuration) {

      if(digitalRead(botonPin) != HIGH){
      unsigned long currentMillis = actualMillis;
      puzzle1LedRgbStatus = true;

      if (currentMillis - previousMillis >= colorDuration) {

        previousMillis = currentMillis;

        colorIndex = colorIndex + 1;
        if(colorIndex == 4){
          colorIndex = 0;
        }

        analogWrite(redPin, colors[colorIndex][0]);
        analogWrite(greenPin, colors[colorIndex][1]);
        analogWrite(bluePin, colors[colorIndex][2]);
        currentColor[0] = colors[colorIndex][0];
        currentColor[1] = colors[colorIndex][1];
        currentColor[2] = colors[colorIndex][2];

      }
        if(currentColor[0] == colorWinner[0]&&currentColor[1] == colorWinner[1]&&currentColor[2] == colorWinner[2]){
          // Serial.println("Ganaste");
          
        }else{
          // Serial.println("Perdiste");
          
        }
      }else{
        if(puzzle1LedRgbStatus){

        if(currentColor[0] == colorWinner[0]&&currentColor[1] == colorWinner[1]&&currentColor[2] == colorWinner[2]){
          // Serial.println("Ganaste");
          puzzle1LedRgbStatus = false;
          
        }else{
          // Serial.println("Perdiste");
          analogWrite(redPin, 0);
          analogWrite(greenPin, 0);
          analogWrite(bluePin, 0);
          vidas--;
          puzzle1LedRgbStatus = false;
          

        }
        }
      }
      }
      //---------------------------rgb 1 boton----------------------------------
      */
      contadorJuegos++;
      break;
    
    case 1:
      
      //---------------------------simon dice----------------------------------
      if (sequenceLength < 9) { // Si el largo de la secuencia alcanza 8, se pasa al siguiente puzzle
        switch (gameState) {
          case SHOW_SEQUENCE:
            showSequence();
            break;
          case WAIT_INPUT:
            readPlayerInput();
            break;
          case WAIT_SUCCESS_MELODY:
            waitSuccessMelody();
            break;
          case PLAY_SUCCESS_MELODY:
            playSuccessMelody();
            break;
          case LOSE:
            handleErrorSound();
            break;
          case PAUSE_BEFORE_NEXT:
            if (millis() - lastActionTime >= 800) {
              startNewRound();
            }
            break;
          }
      }else{
          contadorJuegos++;
      }
      //---------------------------simon dice----------------------------------
      break;
      
    case 2:
      /*--------------------------- cortar cables ----------------------------------*/
      cortarCables();
      break;
      /*--------------------------- cortar cables ----------------------------------*/
    default:
      break;
    }
  }

  //=============== Finalizamos el Juego =============== 
  if (tiempoTotal == 0 && !juegoFinalizado) {
    digitalLCD.clear();
    digitalLCD.setCursor(0, 0);
    digitalLCD.print("Perdiste! :( :(");
    digitalLCD.setCursor(0, 1);
    digitalLCD.print(" No hay Tiempo");
    juegoFinalizado = true;
  }

  if (vidas == 0 && !juegoFinalizado) {
    digitalLCD.clear();
    digitalLCD.setCursor(0, 0);
    digitalLCD.print("Perdiste! :( :(");
    digitalLCD.setCursor(0, 1);
    digitalLCD.print("  No hay Vidas");
    juegoFinalizado = true;
  }

  //=============== Reiniciamos el Juego =============== 
  if (juegoFinalizado && digitalRead(botonPin) == LOW && !estadoBotonMenu) {
    estadoBotonMenu = true;

    //Si pueden, limpien las variables aca jeje
    juegoFinalizado = false;
    juegoIniciado = false;
    vidas = 3;
    tiempoTotal = 0;
    digitalLCD.clear();
  }

  if (digitalRead(botonPin) == HIGH) {
    estadoBotonMenu = false;
  }

}

void seleccionarDificultad() {
  int lectura = analogRead(potenciometroPin);
  int nuevaDificultad;

  // Valor del Potenciometro: 0 al 1023, se divide por 3
  // Dificultad 1 de 0 a 341 / Dificultad 2 de 341 a 682 / Dificultad 3 de 682 a 1023
  if (lectura < 341) {
    nuevaDificultad = 1;
  } else if (lectura < 682) {
    nuevaDificultad = 2;
  } else {
    nuevaDificultad = 3;
  }

  if (nuevaDificultad != dificultad) {
    dificultad = nuevaDificultad;
    digitalLCD.clear();
  }

  digitalLCD.setCursor(0, 0);
  digitalLCD.print("Selecciona nivel");
  digitalLCD.setCursor(0, 1);
  digitalLCD.print("Dificultad ");
  digitalLCD.print(dificultad);
  
  /* Manera de ver el valor del Potenciometro
  Serial.print("Valor Potenciometro: ");
  Serial.println(lectura);
  */

  if (digitalRead(botonPin) == LOW && !estadoBotonMenu) {
    estadoBotonMenu = true;
    juegoIniciado = true;

    switch (dificultad) {
      case 1: 
        tiempoTotal = 5UL * 60UL * 1000UL; // 5 minutos
        break;  
      case 2: 
        tiempoTotal = 4UL * 60UL * 1000UL; // 4 minutos
        break;  
      case 3: 
        tiempoTotal = 3UL * 60UL * 1000UL; // 3 minuto
        break;  
    }

    digitalLCD.clear();
    mostrarEstado();
  }

  if (digitalRead(botonPin) == HIGH) {
    estadoBotonMenu = false;
  }
}
/*
void mostrarTiempo() {
  unsigned long segundosTotales = tiempoTotal / 1000;
  int minutos = segundosTotales / 60;
  int segundos = segundosTotales % 60;

  digitalLCD.setCursor(0, 0);
  digitalLCD.print("  Tiempo ");
  if (minutos < 10) digitalLCD.print("0");
  digitalLCD.print(minutos);
  digitalLCD.print(":");
  if (segundos < 10) digitalLCD.print("0");
  digitalLCD.print(segundos);
}

void mostrarVidas(int vidas) {
  digitalLCD.setCursor(0, 1);
  digitalLCD.print(" Vidas ");
  for (int i = 0; i < vidas; i++) {
    digitalLCD.print("<3 ");
  }
  for (int i = vidas; i < 3; i++) {
    digitalLCD.print("   ");
  }
}
*/

void mostrarEstado() {
  digitalLCD.setCursor(0, 0);

  unsigned long segundosTotales = tiempoTotal / 1000;
  int minutos = segundosTotales / 60;
  int segundos = segundosTotales % 60;

  if (minutos < 10) digitalLCD.print('0');
  digitalLCD.print(minutos);
  digitalLCD.print(':');
  if (segundos < 10) digitalLCD.print('0');
  digitalLCD.print(segundos);
  
  int columna = 5;
  digitalLCD.setCursor(columna, 0);
  digitalLCD.print(" <--> ");
  for (int i = 0; i < vidas; i++) { //Con este For agregamos corazones
    digitalLCD.write((uint8_t)0); 
    digitalLCD.print(' ');  
  }
  for (int i = vidas; i < 3; i++) {
    digitalLCD.print(' ');  
  }
}

//Funciones simon dice
void showSequence() {
  if (showIndex < sequenceLength) {
    if (!showing && millis() - lastActionTime >= pauseBetweenLights) {
      int color = sequence[showIndex];
      setColorByIndex(color);
      tone(BUZZER_PIN, tones[color]);
      showing = true;
      lastActionTime = millis();
    } else if (showing && millis() - lastActionTime >= lightDuration) {
      setColorByIndex(-1);
      noTone(BUZZER_PIN);
      showing = false;
      showIndex++;
      lastActionTime = millis();
    }
  } else {
    gameState = WAIT_INPUT;
    showIndex = 0;
    playerIndex = 0;
  }
}

void readPlayerInput() {
  if (millis() - lastButtonTime < debounceDelay) return;

  for (int i = 0; i < 4; i++) {
    if (digitalRead(BUTTONS[i]) == HIGH) {
      lastButtonTime = millis();
      setColorByIndex(i);
      tone(BUZZER_PIN, tones[i]);
      showing = true;
      lastActionTime = millis();

      if (i == sequence[playerIndex]) {
        playerIndex++;
        if (playerIndex >= sequenceLength) {
          gameState = WAIT_SUCCESS_MELODY;
          lastActionTime = millis();
        }
      } else {
        gameState = LOSE;
        tone(BUZZER_PIN, 200); // tono de error
        errorSoundPlaying = true;
        errorSoundStart = millis();
      }
      return;
    }
  }

  if (showing && millis() - lastActionTime >= lightDuration) {
    setColorByIndex(-1);
    noTone(BUZZER_PIN);
    showing = false;
  }
}

void waitSuccessMelody() {
  if (millis() - lastActionTime >= lightDuration) {
    setColorByIndex(-1);
    noTone(BUZZER_PIN);
    gameState = PLAY_SUCCESS_MELODY;
    melodyIndex = 0;
    melodyStartTime = millis();
  }
}

void playSuccessMelody() {
  if (melodyIndex < successLength) {
    if (millis() - melodyStartTime >= successDurations[melodyIndex]) {
      noTone(BUZZER_PIN);
      melodyIndex++;
      melodyStartTime = millis();
      if (melodyIndex < successLength) {
        tone(BUZZER_PIN, successMelody[melodyIndex]);
      }
    } else if (melodyIndex == 0 && (millis() - melodyStartTime == 0)) {
      tone(BUZZER_PIN, successMelody[melodyIndex]);
    }
  } else {
    noTone(BUZZER_PIN);
    gameState = PAUSE_BEFORE_NEXT;
    lastActionTime = millis();
  }
}

void handleErrorSound() {
  if (errorSoundPlaying && millis() - errorSoundStart >= errorSoundDuration) {
    noTone(BUZZER_PIN);
    errorSoundPlaying = false;
    resetGame();
  }
}

void startNewRound() {
  sequence[sequenceLength] = random(0, 4);
  sequenceLength++;
  gameState = SHOW_SEQUENCE;
  showIndex = 0;
  showing = false;
  lastActionTime = millis();
}

void resetGame() {
  sequenceLength = 0;
  vidas--;
  gameState = PAUSE_BEFORE_NEXT;
  lastActionTime = millis();
}

void setColorByIndex(int index) {
  if (index >= 0 && index < 4) {
    analogWrite(redPin, colors[index][0]);
    analogWrite(greenPin, colors[index][1]);
    analogWrite(bluePin, colors[index][2]);
  } else {
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
  }
}

/*--------------------------- cortar cables ----------------------------------*/

void cortarCables() { // TODO: Agregar dificultades
  // Se elige un modo al azar y se toma en cuenta los pasos a seguir
  int pinEsperado = ordenCorrecto[valorRandom][pasoActual];

  // Muestro el valor random en el LCD
  digitalLCD.setCursor(0,1);
  digitalLCD.print("El codigo es: ");
  digitalLCD.setCursor(14,1);
  digitalLCD.print(valorRandom);

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