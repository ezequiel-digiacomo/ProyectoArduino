
unsigned long tiempoAnterior = 0;

const int LEDS[] = {13,12,11,10};
bool ledsStates[] = {false,false,false,false};
const int BUTTONS[] = {7,6,5,4};
int vals[] = {0,0,0,0};
bool sePuedePresionar = true;
int val;
int val2;
int val3;
int val4;

void setup(){
  for(int i = 0; i<sizeof(LEDS);i++){
  	pinMode(LEDS[i],OUTPUT);
  }
  for(int f = 0; f<sizeof(BUTTONS);f++){
  	pinMode(BUTTONS[f],INPUT);
  }
}
void loop(){

  val = digitalRead(BUTTONS[0]);
  val2 = digitalRead(BUTTONS[1]);
  val3 = digitalRead(BUTTONS[2]);
  val4 = digitalRead(BUTTONS[3]);
  

    if(sePuedePresionar && (val == HIGH || val2 == HIGH || val3 == HIGH || val4 == HIGH)){
    sePuedePresionar = false;
    if(val==HIGH){

      	ledsStates[0] = !ledsStates[0];
        ledsStates[1] = !ledsStates[1];
    	digitalWrite(LEDS[0],ledsStates[0]);
		digitalWrite(LEDS[1],ledsStates[1]);
    }
      if(val2==HIGH){
      	ledsStates[0] = !ledsStates[0];
        ledsStates[1] = !ledsStates[1];
      	ledsStates[2] = !ledsStates[2];
    	digitalWrite(LEDS[0],ledsStates[0]);
		digitalWrite(LEDS[1],ledsStates[1]);
      	digitalWrite(LEDS[2],ledsStates[2]);
    }
      if(val3==HIGH){
      	ledsStates[1] = !ledsStates[1];
        ledsStates[2] = !ledsStates[2];
      	ledsStates[3] = !ledsStates[3];
    	digitalWrite(LEDS[1],ledsStates[1]);
		digitalWrite(LEDS[2],ledsStates[2]);
      	digitalWrite(LEDS[3],ledsStates[3]);
    }
        if(val4==HIGH){
      	ledsStates[2] = !ledsStates[2];
        ledsStates[3] = !ledsStates[3];
    	digitalWrite(LEDS[2],ledsStates[2]);
		digitalWrite(LEDS[3],ledsStates[3]);
    }
    val=LOW;
    val2=LOW;
    val3=LOW;
    val4=LOW;
  }else{
    if(sePuedePresionar == false){
  
    	if(millis() <= (tiempoAnterior+500)){
  		sePuedePresionar = false;
  		}else{
  		sePuedePresionar = true;
          tiempoAnterior = millis();
  		}
    }
  }
}