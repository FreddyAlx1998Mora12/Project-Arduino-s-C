#include <Keypad.h>

// Implementar la libreria a trabajar en este caso es teclado matricial 4x4 keypad
// Declarar en variable const el numero de filas y columnas conectadas

const byte ROWS = 2;
const byte COLS = 4;

// en un array dibujar la posicion de los keypad que vamos a utilizar para imprimir
/* array Keys tendra [filas][columnas] = {{fila1col1, fila1col2, fila1col3}, 
                                          {fila2col1, fila2col2, fila2col3},
                                          {fila3col1, fila3col2, fila3col3}
                                          };
*/ 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6', 'B'}
};

/* Al tener un teclado matricial con varias entradas que conectan al arduino se debe declarar cada fila 
  en cada pin que se conecto, por ejemplo
  pinesFila[numerodefilasdeclarad] = {valorpinfila1,valorpinfila2, valorpinfila3}
*/ 
byte rowsPins[ROWS] = {7,8};
byte colsPins[COLS] = {13,12,11,10};

Keypad keypd = Keypad(makeKeymap(keys),rowsPins,colsPins,ROWS, COLS);

int coordenadaX = 0;
int coordenadaY = 0;

bool flag = true;
int contador = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  char teclaKeypad = keypd.getKey();

  if(teclaKeypad){
    Serial.print(teclaKeypad);
  }

}
