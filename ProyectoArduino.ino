#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define botonPin 2
#define potenciometroPin A0 

LiquidCrystal_I2C digitalLCD(0x27, 16, 2);

unsigned long tiempoTotal;
unsigned long restarTiempo = 0; 
bool estadoBotonMenu = false;
bool juegoFinalizado = false;
int vidas = 3;
int dificultad = 1;
bool juegoIniciado = false;

void setup() {
  digitalLCD.begin(16, 2);
  Serial.begin(9600);
  pinMode(botonPin, INPUT_PULLUP);
  pinMode(potenciometroPin, INPUT);
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
    mostrarVidas(vidas);

    if (actualMillis - restarTiempo >= 1000 && tiempoTotal > 0) {
      tiempoTotal -= 1000;
      restarTiempo = actualMillis;
      mostrarTiempo();
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
    mostrarTiempo();
  }

  if (digitalRead(botonPin) == HIGH) {
    estadoBotonMenu = false;
  }
}

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

