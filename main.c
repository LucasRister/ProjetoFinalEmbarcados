#include <Arduino_FreeRTOS.h>

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


//funcao
  for (;;) // A Tarefa nunca retorna ou sai.
  {


  }
}
