// https://www.tinkercad.com/things/eyVrCm9FVkO-cortar-cables?sharecode=5TrbO6dEMBiWb5AyjUhtVOFcYELgAcqe-cOzigVF5PI

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
  delay(1000);  // Podria ser activada con un sensor que detecte cuando se mueve
}

void loop() {
  // Se pone en una variable la lectura digital
  int cable1 = digitalRead(2);
  int cable2 = digitalRead(3);
  int cable3 = digitalRead(4);
  int cable4 = digitalRead(5);

  // Muestro el valor en el serial
  Serial.println(cable3);

  // Si se corta el cable 3 explota la bomba
  if (cable3 == HIGH) {
    digitalWrite(13, HIGH);
    Serial.write("fiumba");
    while (true) {
      /* nada */;
    }
  }
}