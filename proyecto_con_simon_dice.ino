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
int currentColor[3] = {0, 0, 0};
int colorWinner[3] = {255, 0, 0}; //rojo

bool puzzle1LedRgbStatus = false;
//--------------rgb 1 boton----------------

//--------------simon dice-----------------
#define MAX_SECUENCIA 8
int longitudSecuencia = 5; // cantidad de pasos a memorizar
int secuencia[MAX_SECUENCIA];
int indiceMostrar = 0;
int indiceJugador = 0;
bool mostrandoSecuencia = false;
bool esperandoJugador = false;
bool ledEncendido = false;
bool esperandoApagarColor = false;
unsigned long ultimoCambio = 0;
unsigned long ultimoTiempoColorMostrado = 0;
int colorActualJugador = -1;

const unsigned long tiempoEncendido = 600; // para el led en secuencia
const unsigned long tiempoApagado = 700; // para el led en secuencia

// Pines de botones para Simón Dice (Rojo, Azul, Verde, Amarillo)
const int BUTTONS[4] = {13, 12, 8, 7};

int roundActual = 1;  // Cuántos pasos debe recordar el jugador (empieza en 1)


unsigned long tiempoError = 0;
bool esperandoPorError = false;
const unsigned long duracionError = 2000;

bool esperandoEntreRondas = false;
unsigned long tiempoEsperaEntreRondas = 0;

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

  for (int i = 0; i < 4; i++) {
    pinMode(BUTTONS[i], INPUT);  // o INPUT_PULLUP si tus botones van a GND
  }

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

        if (esperandoPorError) {
          if (millis() - tiempoError >= duracionError) {
            esperandoPorError = false;
            mostrandoSecuencia = true;
            esperandoJugador = false;
            indiceMostrar = 0;
            indiceJugador = 0;
            ultimoCambio = millis();
            ledEncendido = false;
          }
          return; // Evita que siga el flujo hasta que termine el tiempo
        }

        if (!mostrandoSecuencia && !esperandoJugador) {
          iniciarSimon();
        }

        if (mostrandoSecuencia) {
          mostrarSecuencia();
        } else if (esperandoJugador) {
          leerJugador();
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
    longitudSecuencia = 4; // cantidad de pasos a memorizar
  } else if (lectura < 682) {
    nuevaDificultad = 2;
    longitudSecuencia = 6; // cantidad de pasos a memorizar
  } else {
    nuevaDificultad = 3;
    longitudSecuencia = 8; // cantidad de pasos a memorizar
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


/*--------------------------- cortar cables ----------------------------------*/

void cortarCables() { // TODO: Agregar dificultades
  // Se elige un modo al azar y se toma en cuenta los pasos a seguir
  int pinEsperado = ordenCorrecto[valorRandom][pasoActual];

  // Muestro el valor random en el LCD
  digitalLCD.setCursor(0, 1);
  digitalLCD.print("El codigo es: ");
  digitalLCD.setCursor(14, 1);
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
      digitalLCD.setCursor(0, 0);
      digitalLCD.print("Desactivada!");

      // Luz verde
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH);
      digitalWrite(bluePin, LOW);
      while (true) {
        /* nada */
      }
    }
  }

  // Reviso si se corto un cable fuera de orden
  for (int i = 0; i < 4; i++) {
    int pin = ordenCorrecto[valorRandom][i];
    if (i != pasoActual && !cableCortado[i] && digitalRead(pin) == HIGH) {
      digitalLCD.clear();
      digitalLCD.setCursor(0, 0);
      digitalLCD.print("nt"); // Mensaje cuando explota

      // Luz roja
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, LOW);
      while (true) {
        /* nada */
      }
    }
  }
}

//==========================================================
void iniciarSimon() {
  secuencia[0] = random(0, 4); // Solo un color al principio
  roundActual = 1;
  indiceMostrar = 0;
  indiceJugador = 0;
  mostrandoSecuencia = true;
  esperandoJugador = false;
  ultimoCambio = millis();
  ledEncendido = false;
  esperandoApagarColor = false;
}

void mostrarSecuencia() {
  static bool esperandoPrimerSegundo = true;
  static bool esperandoSegundoSegundo = false;
  static bool mostrando = false;
  static unsigned long tiempoEspera = 0;
  unsigned long ahora = millis();

  if (esperandoPrimerSegundo) {
    tiempoEspera = ahora;
    esperandoPrimerSegundo = false;
    esperandoSegundoSegundo = true;
    return;
  }

  if (esperandoSegundoSegundo && ahora - tiempoEspera >= 1000) {
    apagarColor();
    tiempoEspera = ahora;
    esperandoSegundoSegundo = false;
    mostrando = true;
    return;
  }

  if (mostrando && ahora - tiempoEspera >= 1000) {
    if (!ledEncendido && ahora - ultimoCambio >= tiempoApagado) {
      int color = secuencia[indiceMostrar];
      setColor(color);
      ledEncendido = true;
      ultimoCambio = ahora;
    }

    if (ledEncendido && ahora - ultimoCambio >= tiempoEncendido) {
      apagarColor();
      ledEncendido = false;
      ultimoCambio = ahora;
      indiceMostrar++;
    }

    if (indiceMostrar >= roundActual) {
      mostrandoSecuencia = false;
      esperandoJugador = true;
      indiceJugador = 0;
      indiceMostrar = 0;

      // Reset para la próxima vez
      esperandoPrimerSegundo = true;
      esperandoSegundoSegundo = false;
      mostrando = false;
    }
  }
}




void leerJugador() {
  unsigned long ahora = millis();

  static unsigned long ultimoBotonPresionado = 0;
  const unsigned long tiempoAntirrebote = 1000;

  if (esperandoApagarColor) {
    if (ahora - ultimoTiempoColorMostrado >= 200) {
      apagarColor();
      esperandoApagarColor = false;
      colorActualJugador = -1;
    }
    return;
  }

  for (int i = 0; i < 4; i++) {
    if (digitalRead(BUTTONS[i]) == HIGH && (ahora - ultimoBotonPresionado > tiempoAntirrebote)) {
      ultimoBotonPresionado = ahora;

      colorActualJugador = i;
      setColor(i);
      esperandoApagarColor = true;
      ultimoTiempoColorMostrado = ahora;

      if (i == secuencia[indiceJugador]) {
        indiceJugador++;
        if (indiceJugador >= roundActual) {
          // Acertó la ronda, agregamos un nuevo color
          if (roundActual < longitudSecuencia) {
            secuencia[roundActual] = random(0, 4);
            roundActual++;
            mostrandoSecuencia = true;
            esperandoJugador = false;
            indiceMostrar = 0;
            ultimoCambio = millis();
            ledEncendido = false;
          } else {
            // Ganó el juego
            digitalLCD.clear();
            digitalLCD.setCursor(0, 0);
            digitalLCD.print("Completado!");
            contadorJuegos++; // pasa al siguiente juego
          }
        }
      } else {
        // Falló
        vidas--;
        digitalLCD.clear();
        digitalLCD.setCursor(0, 0);
        digitalLCD.print("Incorrecto!");

        esperandoPorError = true;
        tiempoError = millis();
        apagarColor();  // Apagamos el LED
      }

      break; // evitar múltiples detecciones simultáneas
    }
  }
}


void setColor(int index) {
  analogWrite(redPin, colors[index][0]);
  analogWrite(greenPin, colors[index][1]);
  analogWrite(bluePin, colors[index][2]);
}

void apagarColor() {
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
}
