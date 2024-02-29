#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

typedef struct {
  char message[100];
  int blink_count;
}q_msg;

static const uint8_t msg_queue_len = 5;
static QueueHandle_t qHandler1, qHandler2;
void vListenerTask(void *pvParameters){
  char string[100];
  q_msg blink_msg;
  for(;;){
    int i = 0, blink_rate = 0, j, temp;
    while(Serial.available()){
      string[i] = Serial.read();
      temp = string[i]-48;
      if( (temp>=0) && (temp<10) )  blink_rate = blink_rate*10 + temp;
      else if (string[i] == 10){
        if(blink_rate!=0){ 
          if( xQueueSend(qHandler1, (void *)&blink_rate, 10) != pdTRUE){
            Serial.println("Queue Full");
          }
        }
        for(int j=0;string[j]!=10; j++){
          Serial.print(string[j]);
        }
        Serial.println();
        break;
      }
        i++; 
  }
  if(xQueueReceive(qHandler2, (void *)&blink_msg, 0) == pdTRUE) //Serial.println("Queue 2 is empty"); 
  {
    Serial.print(blink_msg.message);
    Serial.print(" and ");
    Serial.println(blink_msg.blink_count);
  }
  }
}

void vBlinkTask(void *pvParameters){
  int blink_count = 0;
  int blink_rate = 10;
  int report_frequency = 100;
  for(;;){
    digitalWrite(LED_BUILTIN, HIGH);
    vTaskDelay(blink_rate/portTICK_PERIOD_MS);
    digitalWrite(LED_BUILTIN, LOW);
    vTaskDelay(blink_rate/portTICK_PERIOD_MS);
    blink_count++;
    if(blink_count%report_frequency==0){
      q_msg message ={"Total number of blinks: ", blink_count};
      if(xQueueSend(qHandler2, (void *)&message, 100) != pdTRUE){
        Serial.println("Queue 2 Full");
      }
    }
    if( xQueueReceive(qHandler1, (void *)&blink_rate, 0) != pdTRUE){}  //Serial.println("Queue 1 is empty");
  }
}



void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  vTaskDelay(500/portTICK_PERIOD_MS);
  Serial.println("Setting up Queues");
  qHandler2 = xQueueCreate(msg_queue_len, sizeof(q_msg));
  qHandler1 = xQueueCreate(msg_queue_len, sizeof(int));
  Serial.println("Starting Tasks");
  xTaskCreatePinnedToCore(
                          vBlinkTask,           //Task Definition
                          "Task A",             //Task Name
                          2048,                 //Stack Size
                          NULL,                 //Task paremeters
                          1,                    //Task Priority
                          NULL,                 //Task Handle
                          app_cpu); 

  xTaskCreatePinnedToCore(
                          vListenerTask,        //Task Definition
                          "Task C",             //Task Name
                          2048,                 //Stack Size
                          NULL,                 //Task paremeters
                          1,                    //Task Priority
                          NULL,                 //Task Handle
                          app_cpu);

}

void loop() {
  
}
