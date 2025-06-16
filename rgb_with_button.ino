
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;
#define button 4

const long colorDuration = 1000; 


unsigned long previousMillis = 0;
int colorIndex = 0;

int colors[4][3] = {
  {255, 0, 0}, //rojo
  {255, 255, 0}, //amarillo
  {0, 255, 0}, //verde
  {0, 0, 255}, //azul
};
int currentColor[3] = {0,0,0};
int colorWinner[3] = {255,0,0}; //rojo 

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(button, INPUT);

  analogWrite(redPin, colors[colorIndex][0]);
  analogWrite(greenPin, colors[colorIndex][1]);
  analogWrite(bluePin, colors[colorIndex][2]);
  Serial.begin(9600);
}

void loop() {
  if(digitalRead(button) == HIGH){
  unsigned long currentMillis = millis();

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
  }else{
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
    if(currentColor[0] == colorWinner[0]&&currentColor[1] == colorWinner[1]&&currentColor[2] == colorWinner[2]){
      Serial.println("Ganaste");
    }else{
      Serial.println("Perdiste");
    }

  }
}