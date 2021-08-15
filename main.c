#include <Arduino_FreeRTOS.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// define as tarefas
void TaskConvert( void *pvParameters );

void setup() {
  
  // inicializa comunicacao serial a 9600 bits por segundos
  Serial.begin(9600);
  
  while (!Serial) {
    ;
  }

  // Configura as tarefas para rodarem independentes
  xTaskCreate(
    TaskConvert
    ,  "Convert"   // Nome da tarefa
    ,  128  // Tamanho da pilha
    ,  NULL
    ,  2  // Prioridade, em que 3 (configMAX_PRIORITIES - 1) para prioridade maxima, e 0 para a prioridade mais baixa.
    ,  NULL );

}

void loop()
{

}

/*--------------------------------------------------*/
/*--------------------- Tarefas --------------------*/
/*--------------------------------------------------*/

void TaskConvert(void *pvParameters)
{
  (void) pvParameters;

//variaveis
/*BUTTONS*/
int pinbtn000001 = 19;
int pinbtn000010 = 18;
int pinbtn000100 = 17;
int pinbtn001000 = 16;
int pinbtn010000 = 15;
int pinbtn100000 = 14;
int btnsend 10;
/*BUTTONSSTATE*/
int pinbtnState000001 = 0;
int pinbtnState000010 = 0;
int pinbtnState000100 = 0;
int pinbtnState001000 = 0;
int pinbtnState010000 = 0;
int pinbtnState100000 = 0;
void setup() {

    /* SET MODE BUTTONS */
    pinMode(pinbtn000001,INPUT);
    pinMode(pinbtn000010,INPUT);
    pinMode(pinbtn000100,INPUT);
    pinMode(pinbtn001000,INPUT);
    pinMode(pinbtn010000,INPUT);
    pinMode(pinbtn100000,INPUT);
    pinMode(btnSend,INPUT);
    
    
    //define o número de colunas e linhas do LCD
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
}
void setState(int pin){
  switch (pin) {
    case 14:
        pinbtnState100000 = ( pinbtnState100000 == 0 ) ? 1 : 0;
        delay(200);
        break;
    case 15:
      	pinbtnState010000 = ( pinbtnState010000 == 0 ) ? 1 : 0;
    	  delay(200);
        break;
    case 16:
      	pinbtnState001000 = ( pinbtnState001000 == 0 ) ? 1 : 0;
    	  delay(200);
        break;
    case 17:
      	pinbtnState000100 = ( pinbtnState000100 == 0 ) ? 1 : 0;
    	  delay(200);
        break;
    case 18:
      	pinbtnState000010 = ( pinbtnState000010 == 0 ) ? 1 : 0;
    	  delay(200);
        break;
    case 19:
      	pinbtnState000001 = ( pinbtnState000001 == 0 ) ? 1 : 0;
    	  delay(200);
        break;
    default:
    break;
  }

}

//funcao
  for (;;) // A Tarefa nunca retorna ou sai.

//Algoritmo conversor bin pra dec
int BinToDec(char *s) {
  int result = 0;
  while(*s) {
    result <<= 1;
    if(*s++ == '1') result |= 1;
  }
  return result;
}

void convert(){
  	lcd.clear();
  	printBin();
  	char binary [7];

  
  	String strBinario = (
      String(pinbtnState100000)+
      String(pinbtnState010000)+
      String(pinbtnState001000)+
      String(pinbtnState000100)+
      String(pinbtnState000010)+
      String(pinbtnState000001)
      );

  	strBinario.toCharArray(binary,7);
  
  	int decimal = BinToDec(binary);
  	lcd.setCursor(0, 1);
  	lcd.print("Decimal:");
    lcd.print(decimal);
 }
void printBin(){
  lcd.setCursor(0, 0);
  lcd.print("Binario:");
  //Print binarios
  lcd.print(pinbtnState100000);
  lcd.print(pinbtnState010000);
  lcd.print(pinbtnState001000);
  lcd.print(pinbtnState000100);
  lcd.print(pinbtnState000010);
  lcd.print(pinbtnState000001);
}

void loop() {
  	//Pressionou botão
  	if(digitalRead(pinbtn000001) == LOW) setState(pinbtn000001);
  	if(digitalRead(pinbtn000010) == LOW) setState(pinbtn000010);
  	if(digitalRead(pinbtn000100) == LOW) setState(pinbtn000100);
  	if(digitalRead(pinbtn001000) == LOW) setState(pinbtn001000);
  	if(digitalRead(pinbtn010000) == LOW) setState(pinbtn010000);
  	if(digitalRead(pinbtn100000) == LOW) setState(pinbtn100000);
  	printBin();

  //ENVIAR
    if(digitalRead(btnSend) == LOW)convert();
  	
  	delay(50);
}
