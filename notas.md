## Notas clase 22.05
Hay que calibrar la fotoresistencia: el valor de referencia a la masa va a ser el nivel de luz actual  
  
Un cable suelto funciona como una antena. Si dejas un sensor con una pata sin conectar, recibe ruido 
  
Resistencia PULLDOWN, tira para abajo, da un cero de verdad. Es una especie de puente para que no esten conectados directo el GND Y 5V. Tengo 0 todo el tiempo que no tengo 1.  
  
~~~  
(if variable == high)  
~~~
  
Tambien hay resistencias pullup, son exactamente al revés  
~~~  
(if variable == low)  
~~~  
  
> Es indistinto cual se use  
 
LOUD WRONG BUZZER SOUND  
  
When to Use:  
- Pull-up:  
	Often used with pushbuttons or switches where the default state is low and the desired state is high when the button is pressed.  
- Pull-down:  
	Often used with open-collector or open-drain outputs where the default state is high and the desired state is low when the output is active.  
  
> In essence, pull-up and pull-down resistors are used to provide a predictable default state for a pin, preventing it from floating and potentially causing unpredictable behavior in a digital circuit.