#include <Servo.h>

Servo servo1;
//Entrada de Potenciometro = Entrada, ya que mediante la regulacion esta corrriente entrara a la placa arduino
//y se la remitira al servo.
int potPin = A0;

//declarar la variable del servopin
int servoPin = 9;
int angulo = 0;
int potValue_Actual;


void setup() {
  // put your setup code here, to run once:
  // declarar el valor de pin
  servo1.attach(servoPin);
  // declarar el valor del pin de potenciometro 
  // y a su vez como modo de entrada ya que la corriente ingresara a la placa
  pinMode(potPin, INPUT);
  servo1.write(angulo);

}

void loop() {
  // put your main code here, to run repeatedly:
  // Significa que en potValue se va a guardar la variable int de lo que detecta mediante la lectura analogica
  // por el pin declarado.
  potValue_Actual = analogRead(potPin);
  
  angulo = map(potValue_Actual,0, 1023, 0, 180);

  servo1.write(angulo);
  delay(10);

}
