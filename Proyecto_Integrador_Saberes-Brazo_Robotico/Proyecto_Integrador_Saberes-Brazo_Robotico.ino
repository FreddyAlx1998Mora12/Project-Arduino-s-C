#include <SoftwareSerial.h>

#include <Key.h>
#include <Keypad.h>

#include <Servo.h>


//Variables Servo
Servo servoBase, servoCodo, servoAntebrazo, servoPinza;

// servoPin = valor de Pin conectado al arduino
// ang_Servo = valor inicial para el movimiento de brazo segun la parte que toca
// valorAngC = variable opcional para leer la posicion o el grado del servomotor y en base a eso evaluar las condiciones
int servoPinB = 6, ang_ServoB = 90, valorAngB = 0;  //Servo Base 0 - 360
int servoPinA = 3, ang_ServoA = 0, valorAngA;       //Servo Antebrazo
int servoPinC = 5, ang_ServoC = 70, valorAngC;      //Servo Codo
int servoPinP = 9, ang_ServoP = 0, valorAngP;       //Servo Pinza


// Potenciometro
int pin_Potenc = A0;
int anguloPot = 0;
int potValue_Actual;

//Variables Teclado Dimension de la matriz
const byte ROWS = 2;
const byte COLS = 4;

//Matriz teclado real REFERENCIA
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' }
};

//Pines Conexion teclado Matricial
byte rowsPins[ROWS] = { 7, 8 };
byte colsPins[COLS] = { 13, 12, 11, 10 };


// Variable de type object para detectar tecla presionada
Keypad keypd = Keypad(makeKeymap(keys), rowsPins, colsPins, ROWS, COLS);

// Variables para almacenar las coordenadas ingresadas por el usuario
int coordenadaX, valor1;  // valor1 y valor2 variables para comparacion, si son numeros de 0 - 9, guarda en la variable coordenada
int coordenadaY, valor2;

bool flag = true;  // flag para coordenadas x y y en un solo pulsar de tecla
bool datos_XY = true;
int contador = 0;

// int count_text = 0;  // contador para que muestre 1 sola vez datos  de bienvenida al usuario

bool flag_respuesta = true;
int control_program = 0;
String respuesta;


void setup() {
  Serial.begin(9600);

  // Configuracion servos
  servoAntebrazo.attach(servoPinA, 500, 2500);  //cambiar al respectivo servo
  servoCodo.attach(servoPinC, 500, 2500);       //cambiar al respectivo servo
  servoPinza.attach(servoPinP, 500, 2500);
  servoBase.attach(servoPinB, 500, 2500);

  // Configuracion potenciometro
  pinMode(pin_Potenc, INPUT);
}

void loop() {
  // METODO PRINCIPAL DE EJECUCION
  
  control_program = 0;
  while (flag_respuesta) {
    servoAntebrazo.write(ang_ServoA);
    servoCodo.write(ang_ServoC);
    
    servoBase.write(ang_ServoB);
    limpiarBufferEntrada();
    if (control_program == 0) {
      lectura_coordenadas(); // Metodo para lectura de datos
      movimientoServoBase();  // movimiento por coordenada
      control_program = 1;
      limpiarBufferEntrada();  // limpia la serial y los datos existentes.
    }
    if (control_program == 1) {
      // empieza a controlar el brazo...
      char tecla = keypd.getKey(); // obtiene una tecla presionada por teclado matricial
      
      movimientoServoAntebrazo(tecla); // mueve el antebrazo
      movimientoServoCodo(tecla); // mueve el codo
      movimiento_manual_ServoBase(tecla); // mover base
      movimiento_potenciometro_pinza(); // mueve pinza con potenciometro
      
    }

    if(control_program == 2){
      Serial.print("Desea empezar de nuevo con el programa? Elige (S/N): ");
      while (Serial.available() == 0) {
        // Espera que ingrese un dato
      }
      respuesta = Serial.readString();
      if (respuesta == "N" || respuesta == "n") {
        //rompe el bucle
        Serial.println("Gracias por usar nuestro programa..... Buen dia.");
        break;
      } else {
        control_program = 0;
        limpiarBufferEntrada();
      }
    }
  }

  // delay(125);
  //obtiene la tecla presionada de keypad
}

void lectura_coordenadas() {
  // Mostrar el mensaje en Monitor una sola vez
  static int count_text = 0;
  //Logica de lectura
  if (count_text == 0) {

    Serial.println("\r================================================");
    Serial.println("COORDENADAS ESTABLECIDAS");
    Serial.println("================================================");
    Serial.println("X: (Rango de 0 a 9)");
    Serial.println("Y: (Rango de 0 a 4)");
    Serial.println("================================================");


    // Presiona una tecla para la coordenada x
    Serial.println("INGRESE COORDENADAS PARA X.");
    Serial.print("X: ");

    while (true) {
      while (Serial.available() == 0) {
        // Espera que ingrese un dato
      }
      valor1 = Serial.parseInt();
      if (valor1 >= 0 && valor1 <= 9) {
        coordenadaX = valor1;
        Serial.println(coordenadaX);
        break;
      } else {
        Serial.println("Dato incorrecto, ingrese solo número para X entre 0 - 9... digite un numero");
        Serial.print("X: ");
      }
    }

    // coordenada Y
    Serial.println("INGRESE COORDENADAS PARA Y.");
    Serial.print("Y: ");

    limpiarBufferEntrada();

    while (true) {
      while (Serial.available() == 0) {
        // Espera que ingrese un dato
      }
      valor2 = Serial.parseInt();
      if (valor2 >= 0 && valor2 <= 4) {
        coordenadaY = valor2;
        Serial.println(coordenadaY);
        break;
      } else {
        Serial.println("Dato incorrecto, ingrese solo número para Y entre 0 - 4... digite un numero");
        limpiarBufferEntrada();
        Serial.print("Y: ");
      }
    }

    count_text = 1;

    if (count_text == 1) {
      Serial.print("Variable X: ");
      Serial.println(coordenadaX);
      Serial.print("Variable y: ");
      Serial.println(coordenadaY);
      String mensaje = "Vector = [" + String(coordenadaX) + "," + String(coordenadaY) + "]";
      Serial.println(mensaje);
    }
    count_text = 2;
  }
}

void movimientoServoAntebrazo(char tecla2) {
  /*
    Valor maximo_angulo = 90 -> encogido
    Valor minimo_angulo = 0  -> estirado
    Tecla 2 = estirar antebrazo -> encoger, aumentara el valor del angulo de Servo
    Tecla 5 = levantar antebrazo -> estirar, disminuira el valor del angulo de Servo
  */

  switch (tecla2) {
    case '5':
      // delay(10000);
      // Serial.print(tecla2);
      if (ang_ServoA >= 0 && ang_ServoA <= 80) {
        // aumenta, incrementa el angulo de movimiento del antebrazo
        ang_ServoA += 10;
        // delay(100);
        servoAntebrazo.write(ang_ServoA);
        // En terminal visualizo la posicion actual
        // valorAngA = servoAntebrazo.read();
        // Serial.println(valorAngA);
        // Serial.println(ang_ServoC);

        // Logica para cuando baja el Antebrazo, el codo ceda un pequenio movimiento para no forzar
        if (ang_ServoA > 30) {
          //reutilizamos metodo
          movimientoServoCodo('3');
          movimientoServoCodo('3');
        }
      } else {
        // emite un sonido para que sepa que llego al limite maximo
        // o presentale en consola que no puede mover mas
        Serial.print("Alcanzo limite de movimiento");
      }
      valorAngA = servoAntebrazo.read();
      // Serial.println(valorAngA);
      break;

    case '2':
      // Serial.print(tecla2);
      if (ang_ServoA >= 10 && ang_ServoA <= 90) {
        ang_ServoA -= 10;
        servoAntebrazo.write(ang_ServoA);
        if (ang_ServoA < 70) {
          movimientoServoCodo('1');
          movimientoServoCodo('1');
        }
      } else {
        // lo mismo que el anterior
        Serial.println("Alcanzo limite de movimiento");
      }
      break;
  }
}

void movimientoServoCodo(char tecla2) {
  /*
    Valor inicial = 0;
    Valor maximo_angulo = 70 -> encogido
    Valor minimo_angulo = 0  -> estirado
    Tecla 1 = subir codo -> encoger, aumentara el valor del angulo de Servo
    Tecla 3 = bajar codo -> estirar, disminuira el valor del angulo de Servo
  */
  // valorAngC = servoCodo.read();
  // ang_ServoC = valorAngC;
  switch (tecla2) {
    case '1':
      // delay(10000);
      // Serial.println(valorAngC);
      // 1
      // Serial.println(tecla2);
      // Si angulo de Antebrazo es menor a 50 puedes regresar el servo del Codo encojer
      if (ang_ServoA < 50) {
        if (ang_ServoC >= 0 && ang_ServoC <= 65) {
          // aumenta, incrementa el angulo de movimiento del codo
          ang_ServoC += 5;
          // delay(100);
          servoCodo.write(ang_ServoC);
          // En terminal visualizo la posicion actual
          valorAngC = servoCodo.read();
          // Serial.println(valorAngC);
          // Serial.println(ang_ServoC);
        } else {
          // emite un sonido para que sepa que llego al limite maximo
          // o presentale en consola que no puede mover mas
        }
      }
      break;

    case '3':
      // Serial.println(tecla2);
      if (ang_ServoC >= 5 && ang_ServoC <= 70) {
        ang_ServoC -= 5;
        servoCodo.write(ang_ServoC);
      } else {
        // lo mismo que el anterior
        // Serial.println(ang_ServoC);
      }
      break;
  }
}

// Este metodo debo borrar o esperar
void movimiento_manual_ServoBase(char tecla2) {
  /*
    Valor inicial = 0;
    Valor maximo_angulo = 70 -> encogido
    Valor minimo_angulo = 0  -> estirado
    Tecla 1 = subir codo -> encoger, aumentara el valor del angulo de Servo
    Tecla 3 = bajar codo -> estirar, disminuira el valor del angulo de Servo
  */
  // servoCodo.write(ang_ServoP);
  // valorAngC = servoCodo.read();
  // ang_ServoC = valorAngC;
  switch (tecla2) {
    case '4':
      // delay(10000);
      // Serial.println(valorAngC);
      // Serial.println(tecla2);
      if (ang_ServoB >= 0 && ang_ServoB <= 85) {
        // aumenta, incrementa el angulo de movimiento del codo
        ang_ServoB += 5;
        // delay(100);
        servoBase.write(ang_ServoB);
        // En terminal visualizo la posicion actual
        // valorAngP = servoCodo.read();
        // Serial.println(valorAngC);
        // Serial.println(ang_ServoC);
      } else {
        // emite un sonido para que sepa que llego al limite maximo
        // o presentale en consola que no puede mover mas
      }
      break;

    case '6':
      if (ang_ServoB >= 5 && ang_ServoB <= 90) {
        ang_ServoB -= 5;
        servoBase.write(ang_ServoB);
      } else {
        // lo mismo que el anterior
        // Serial.println(ang_ServoC);
      }
      break;
  }
}

void movimiento_potenciometro_pinza() {
  // Movimiento pinza por potenciometro.
  potValue_Actual = analogRead(pin_Potenc);
  // Serial.println("--------------");
  // Serial.print("Valor de potenciometro");
  // Serial.println(potValue_Actual);

  anguloPot = map(potValue_Actual, 0, 1023, 0, 85);

  // Serial.println("--------------");
  // Serial.print("Angulo Potenciometro");
  // Serial.println(anguloPot);

  servoPinza.write(anguloPot);
  delay(10);
}
//Metodo Base
// movimiento Base
void movimientoServoBase() {
  // Serial.println("metodo rotacion");
  // Movimiento segun matriz o coordenadas
  // Coordenadas objetivo (ajusta según tus necesidades)

  float objetivoX = coordenadaX;  // Coordenada X deseada
  float objetivoY = coordenadaY;  // Coordenada Y deseada

  // Ajustar el punto de origen
  float origenX = 8.2;
  float origenY = 0.0;

  // Calcular las diferencias desde el origen
  float deltaX = objetivoX - origenX;
  float deltaY = objetivoY - origenY;

  // Calcular el ángulo en radianes utilizando atan2
  float anguloRad = atan2(deltaY, deltaX);

  // Convertir el ángulo de radianes a grados
  float anguloDeg = anguloRad * 180.0 / PI;

  // Mapear el ángulo a un rango adecuado para el servo
  int anguloServo = map(anguloDeg, -180, 180, 180, 0);
  // anguloServo -= 90;  //revisar aqui
  // Imprimir el ángulo en el monitor serie
  Serial.print("Ángulo del servo para X, Y: ");
  Serial.print(anguloServo);
  Serial.println(" grados");
  Serial.println();
  // Espera un segundo
  delay(1000);
  servoBase.write(anguloServo);
}

void limpiarBufferEntrada() {
  // Asegurémonos de que el buffer de entrada esté limpio
  while (Serial.available() > 0) {
    Serial.read();
  }
}