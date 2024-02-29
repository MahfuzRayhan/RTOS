#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static uint8_t global_resource = 0;
SemaphoreHandle_t mutexHandle;


void vIncrementingTask(void *pvParameters){
  uint8_t local_resource;
  for(;;){
    if (xSemaphoreTake(mutexHandle, 0)==pdTRUE){
    local_resource= global_resource;
    local_resource++;
    vTaskDelay(random(100, 500)/portTICK_PERIOD_MS);
    global_resource = local_resource;
    xSemaphoreGive(mutexHandle);
    Serial.println(global_resource);
    }
    else {
//      Serial.println("Mutex not available");   // Not applicable for portMAX_DELAY
    }
  }
}

void setup() {
  Serial.begin(115200);
  mutexHandle = xSemaphoreCreateMutex();
  Serial.println("Starting Tasks");
  xTaskCreatePinnedToCore(
                          vIncrementingTask,           //Task Definition
                          "Incrementing Task A",             //Task Name
                          1024,                 //Stack Size
                          NULL,                 //Task paremeters
                          1,                    //Task Priority
                          NULL,                 //Task Handle
                          app_cpu); 

  xTaskCreatePinnedToCore(
                          vIncrementingTask,        //Task Definition
                          "Incrementing Task B",             //Task Name
                          1024,                 //Stack Size
                          NULL,                 //Task paremeters
                          1,                    //Task Priority
                          NULL,                 //Task Handle
                          app_cpu);

}

void loop() {
  
}
