#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static const uint8_t msg_queue_len = 5;
static QueueHandle_t msg_queue;

void messagePrintTask(void *pvParameters){
  int item;
  while(1){
    if(xQueueReceive(msg_queue, (void *)&item, 0) == pdTRUE){
      Serial.println(item);
    }
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(57600);
  vTaskDelay(1000/portTICK_PERIOD_MS);


  msg_queue = xQueueCreate(msg_queue_len, sizeof(int));
  xTaskCreatePinnedToCore(messagePrintTask,        //Task Definition
                          "Print Messages",     //Task Name
                          1024,                 //Stack Size
                          NULL,                 //Task paremeters
                          1,                    //Task Priority
                          NULL,                 //Task Handle
                          app_cpu);             //which core to use only
 }



void loop() {
  static int num = 0;
  if(xQueueSend(msg_queue, (void *)&num, 10) != pdTRUE) Serial.println("Queue Full");
  num++;
  vTaskDelay(1000/portTICK_PERIOD_MS);
}
