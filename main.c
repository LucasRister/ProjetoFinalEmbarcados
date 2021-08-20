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