// Declaracion de variables
// Definir leds 
// led_color = numero_pin

int led_red = 8;
int led_yell = 7;
int led_green = 4;

// contador bool
int counter = 1;

void setup() {
  // put your setup code here, to run once:
  // Configuracion
  pinMode(led_yell, OUTPUT);
  pinMode(led_red, OUTPUT);
  pinMode(led_green, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  semaforo_bienvenida();
  iniciar_semaforo();
}


// Metodos personalizados
void semaforo_bienvenida(){
  if (counter == 1){
    
  	//encendemos los leds como bienvenida
    digitalWrite(led_red, HIGH);
  	delay(1000);
    digitalWrite(led_red, LOW);
    digitalWrite(led_yell, HIGH);
    delay(1000);
    digitalWrite(led_yell, LOW);
    digitalWrite(led_green, HIGH);
    delay(1000);
    digitalWrite(led_green, LOW);
    
    for (int i = 0; i <= 3; i++)
    {
      //Enciende 3 veces
      digitalWrite(led_red, HIGH);
      digitalWrite(led_yell, HIGH);
      digitalWrite(led_green, HIGH);
      
      //esperamos 3.5 seg para apagar
      delay(2100);
      
      //apaga todos los leds
      digitalWrite(led_red, LOW);
      digitalWrite(led_yell, LOW);
      digitalWrite(led_green, LOW);
      delay(1500);
    }
  }
  
  counter = 0;
}

void iniciar_semaforo(){

    // inicia
  digitalWrite(led_red, HIGH);
  digitalWrite(led_yell, LOW);
  digitalWrite(led_green, LOW);
  
  delay(4000); // Wait for 4000 millisecond(s)
  digitalWrite(led_red, LOW);
  digitalWrite(led_yell, LOW);
  digitalWrite(led_green, HIGH);
  
  delay(4000); // Wait for 2000 millisecond(s)
  digitalWrite(led_red, LOW);
  digitalWrite(led_yell, HIGH);
  digitalWrite(led_green, LOW);
  delay(1700); // Wait for 3000 millisecond(s)
}
