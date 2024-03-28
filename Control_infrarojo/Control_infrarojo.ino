// #include <IRremote.hpp>
#include <IRremote.h>

// COntrol Remoto IR
// D2 - sensor ir
// D5 - red1
// D6 - red2
// D7 - blue 1
// D8 - blue 2
// D9 - yellw 1
// D10 - yellw 2

int receptor_sensor = 2;
IRrecv irrecv(receptor_sensor);
decode_results codigo;

int led_red = 5;
int led_red2 = 6;
int led_blue = 7;
int led_blue2 = 8;




void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  // Configuracion de leds
  for (int i = led_red; i <= led_blue2; i++) {
    pinMode(i, OUTPUT);
  }

  // Inicializa receptor
  irrecv.enableIRIn();
  Serial.println("Iniciando el receptor IR. Apunta tu control remoto hacia el sensor.");
  Serial.println("Configuración de pines de LED completada.");
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.print("hoola: ");
  if (irrecv.decode(&codigo)) {
    // Serial.print(results.value);
    // test
    Serial.print("Valor de la tecla: ");
    Serial.println(codigo.value, DEC);
    switch (codigo.value) {
      case 16:
      // red 1
        encender_led(5);
        break;

      case 2064:
      // red 2
        encender_led(6);
        break;

      case 1040:
      // blue 
        encender_led(7);
        break;

      case 3088:
      // blue 2
        encender_led(8);
        break;

        case 528:
      // red 1 apaga led
        apagar_led(5);
        break;

      case 2576:
      // red 2
        apagar_led(6);
        break;

      case 1552:
      // blue 
        apagar_led(7);
        break;

      case 3600:
      // blue 2
        apagar_led(8);
        break;

      case 1680:
      // apagar todos
        apagar_todos();
        break;

      case 3344:
      // encender todos
        encender_all_Led();
        break;

      default:
        break;
    }
    delay(500);
    irrecv.resume();  // Recibe siguiente valor
  }
}

void encender_led(int pin) {
  digitalWrite(pin, HIGH);
}

void apagar_led(int pin) {
  digitalWrite(pin, LOW);
}

void encender_all_Led() {
  for (int i = led_red; i <= led_blue2; i++) {
    digitalWrite(i, HIGH);
  }
}

void apagar_todos() {
  for (int i = led_red; i <= led_blue2; i++) {
    digitalWrite(i, LOW);
  }
}
