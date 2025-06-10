// https://www.tinkercad.com/things/eyVrCm9FVkO-cortar-cables?sharecode=5TrbO6dEMBiWb5AyjUhtVOFcYELgAcqe-cOzigVF5PI

long valorRandom;

void setup() {
  // Conexion serial
  Serial.begin(9600);
  Serial.write("\n Armada \n");

  // Configuracion de pines
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);  //INPUT_PULLUP, podria ser
  pinMode(5, INPUT);

  // Si la bomba explota, se prende el led integrado
  pinMode(13, OUTPUT);

  // https://docs.arduino.cc/language-reference/en/functions/random-numbers/random/
  randomSeed(analogRead(0)); // Si el pin analógico 0 está desconectado, el ruido hace que randomSeed() genere números random
  valorRandom = random(4); // Valor random entre 0 y 3
  
  // Muestro el valor random en el serial
  Serial.println(valorRandom);

  delay(1000);  // Podria ser activada con un sensor que detecte cuando se mueve
}

void loop() {
  // Se pone en una variable la lectura digital
  int cable0 = digitalRead(2);
  int cable1 = digitalRead(3);
  int cable2 = digitalRead(4);
  int cable3 = digitalRead(5);
  
  // Diferentes casos
  switch (valorRandom) {
    case 0:
      if (cable0 == HIGH) {
        digitalWrite(13, HIGH);
        Serial.write("fiumba");
        while (true) {
          /* nada */;
        }
      }
      break;
    case 1:
      if (cable1 == HIGH) {
        digitalWrite(13, HIGH);
        Serial.write("fiumba");
        while (true) {
          /* nada */;
        }
      }
      break;
    case 2:
      if (cable2 == HIGH) {
        digitalWrite(13, HIGH);
        Serial.write("fiumba");
        while (true) {
          /* nada */;
        }
      }
      break;
    case 3:
      if (cable3 == HIGH) {
        digitalWrite(13, HIGH);
        Serial.write("fiumba");
        while (true) {
          /* nada */;
        }
      }
      break;
  }
}