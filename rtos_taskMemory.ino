#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

void vTaskExample(void *pvParameter) {
  int a[2]={110110, 120120};
  
  for(;;) {
    Serial.println(xPortGetFreeHeapSize());
    int *start_of_array =  (int*)pvPortMalloc(102*sizeof(int));
    if(start_of_array==NULL) Serial.println("Insufficient Heap");
    else{
      for(int i=0; i< 102; i++){
        start_of_array[i] = i-10;
      }
      Serial.println(*(start_of_array+64));
      Serial.println(xPortGetFreeHeapSize());
    }
    Serial.println(a[0]);
    Serial.println(uxTaskGetStackHighWaterMark(NULL));
    vPortFree(start_of_array);
  }
}


void setup() {
  TaskHandle_t task_example = NULL;
  Serial.begin(115200);
  // Task to run forever
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              vTaskExample,  // Function to be called
              "ID Allocation 1",   // Name of task
              3072,         // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,         // Parameter to pass to function
              1,            // Task priority (0 to configMAX_PRIORITIES - 1)
              NULL,         // Task handle
              app_cpu);     // Run on one core for demo purposes (ESP32 only)
}

void loop() {
  
}
