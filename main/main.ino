#include <LiquidCrystal.h>
#include <stdio.h>
#include <Arduino_FreeRTOS.h>
#include "task.h"
#include <queue.h>
#define tempoDebounce 50

volatile int32_t result =0; //var que recebe dados da entrada
volatile int decimal = 0;   //var que recebe conversao de Bin to Dec

QueueHandle_t queue;
QueueHandle_t queue2;
 
TaskHandle_t process;
TaskHandle_t Show;
TaskHandle_t Input; 

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() {
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  lcd.begin(16, 2);
  Serial.begin(9600);
  
  queue = xQueueCreate(1, sizeof(int));
  queue2 = xQueueCreate(1, sizeof(int));

  xTaskCreate(Input1,  "Input" , 128, NULL, 2 , &Input );
  xTaskCreate(process1,  "process" , 128, NULL, 2 , &process );
  xTaskCreate(Show1,  "Show" , 128, NULL, 1 , &Show );
}

void loop()
{
//Fluxo acontece nas Tasks
}

static void  Input1(void *pvParameters){ //Task para coletar os btns de entrada

int state;
  for(;;){
      int valBotoes = analogRead(A0);
    
      if ((valBotoes < 800) && (valBotoes >= 600)) {
         state = 1; // condicao para o btn SELECT
         xQueueSend(queue, &state, portMAX_DELAY);
         
      } else if ((valBotoes < 600) && (valBotoes >= 400)) {
         state = 2; // condicao para o btn LEFT;
         xQueueSend(queue, &state, portMAX_DELAY);
         
      } else if ((valBotoes < 400) && (valBotoes >= 200)) {
         state = 4; // condicao para o btn UP;
         xQueueSend(queue, &state, portMAX_DELAY);
         
      } else if ((valBotoes < 200) && (valBotoes >= 60)) {
         state = 3; // condicao para o btn DOWN;
         xQueueSend(queue, &state, portMAX_DELAY);
         
      } else if  (valBotoes < 60) {
         state = 5; // condicao para o btn RIGHT;
         xQueueSend(queue, &state, portMAX_DELAY);
         
      } else {
         state = 0; // condicao para o btn EMPTY;
         xQueueSend(queue, &state, portMAX_DELAY);
      }   
  }
}

static void process1(void *pvParameters) // Task para executar as funcoes referente a cada btn
{

  unsigned long delayBotao; //Delay do debounce
  int estadoBotaoAnt = 0; //Var temp para o debounce
  int32_t remainder =0; //var para o resto da divisao do conversor de bin para dec
  int32_t base =1; // var para a base multiplicativa do conversor de bin para dec
  int state1; //var que recebe qual btn foi apertado
  int input =0; //var que determina o que mostrar no display 0 - entrada 1 - conversao
  
  for(;;){
      xQueueReceive(queue, &state1, portMAX_DELAY);
      if ((millis() - delayBotao) > tempoDebounce) {
         if ((state1 != 0) && (estadoBotaoAnt == 0) ) {        
              if(state1 == 2){ //se btn LEFT pressionado 
                result = result*10 + 1; //recebe 1 e pula para o proximo digito
                input = 0; // Seta o display para mostrar os dados de entrada
                xQueueSend(queue2, &input, portMAX_DELAY);
              }
              if(state1 == 5){ // se btn RIGHT pressionado
                result = result*10; // recebe 0 e pula para o proximo digito
                input = 0;  //Seta o display para mostrar os dados de entrada
                xQueueSend(queue2, &input, portMAX_DELAY);
              }
              if(state1 == 3){//se btn UP pressionado convert Bin to Dec
                remainder =0;
                decimal = 0;
                base =1;
                while(result > 0){
                  remainder =result % 10;
                  decimal = decimal + remainder* base;
                  result = result/10;
                  base = base*2;
                }   
                input = 1; // seta o display para mostrar a conversao de bin para dec
                xQueueSend(queue2, &input, portMAX_DELAY);
              }     
            delayBotao = millis();
         }
         if ((state1 == 0) && (estadoBotaoAnt != 0) ) {
            delayBotao = millis();
         }
      }
      estadoBotaoAnt = state1;
      input =0;    // Seta o display para mostrar os dados de entrada
      xQueueSend(queue2, &input, portMAX_DELAY); 
  }
}

static void Show1(void *pvParameters){ // Task para mostrar os dados na tela

  int input1 =0;
  
  for(;;){
    xQueueReceive(queue2, &input1, portMAX_DELAY);
    if(input1 ==0 ){ //Condicao para mostrar os dados da entrada
        lcd.setCursor(0,0);
        lcd.print("Input:");
        lcd.setCursor(2,1);
        lcd.print(result); //Mostra os dados da entrada
    }else if(input1 ==1){//Condicao para mostrar os dados convertidos
        lcd.clear();
        lcd.print("Dec:");
        lcd.setCursor(2,1);
        lcd.print(decimal); //Mostra a conversao
        vTaskDelay(pdMS_TO_TICKS(5000)); //Deixa os dados na tela por 5s
        result =0;  //Reseta a var dos dados convertidos
        lcd.clear(); //Reseta o display
    }
  }
}
