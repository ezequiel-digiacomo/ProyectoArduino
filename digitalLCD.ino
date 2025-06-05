#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define botonPin 2

LiquidCrystal_I2C digitalLCD(0x27, 16, 2); // col: columnas del 0 al 15 - row: filas (tenemos 2) -> setCursor(columna, fila)

unsigned long tiempoRestante = 5UL * 60UL * 1000UL;
unsigned long lastMillis = 0;
bool botonPresionado = false;
bool textoFijoEscrito = false;

void setup() {
  digitalLCD.begin(16, 2);
  pinMode(botonPin, INPUT_PULLUP); 
  mostrarTiempo();
  mostrarVidas();
}

void loop() {
  unsigned long actualMillis = millis();

  if (actualMillis - lastMillis >= 1000 && tiempoRestante > 0) {
    tiempoRestante -= 1000;
    lastMillis = actualMillis;
    mostrarTiempo();
  }

  // Si detecta el Boton Presionado significa que esta en estado LOW
  if (digitalRead(botonPin) == LOW && !botonPresionado) {
    botonPresionado = true;
    if (tiempoRestante >= 15000) {
      tiempoRestante -= 15000;
    } else {
      tiempoRestante = 0;
    }
    mostrarTiempo();
  }

  if (digitalRead(botonPin) == HIGH) {
    botonPresionado = false;
  }
}

void mostrarTiempo() {
  unsigned long segundosTotales = tiempoRestante / 1000;
  int minutos = (int)(segundosTotales / 60);
  int segundos = (int)(segundosTotales % 60);

  digitalLCD.setCursor(0, 0);
  digitalLCD.print("  Tiempo ");
  if (minutos < 10){
  	digitalLCD.print("0");
  }
  digitalLCD.print(minutos);
  digitalLCD.print(":");
  if (segundos < 10){
  	digitalLCD.print("0");
  }
  digitalLCD.print(segundos);
}

void mostrarVidas() {
  digitalLCD.setCursor(0, 1);
  digitalLCD.print(" Vidas <3 <3 <3");
}// El archivo quedó vacío jia
