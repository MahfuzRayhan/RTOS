#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

char message[100];
BaseType_t flag = 0;


void vRecievingTask(void *pvParameter) {
  for(;;) {
    if(Serial.available()){
      int count = 0;
      while(Serial.available()){
        message[count] = Serial.read();
        count++;
      }
//      Serial.print("I said: <");
//      Serial.print(message);
//      Serial.println(">");
      flag = 1;
      vTaskDelay(portTICK_PERIOD_MS);
    }
    else{
//      Serial.println("I said: <Noting>");
      vTaskDelay(portTICK_PERIOD_MS);
    }
  }
}


void vSendingTask(void *pvParameter) {
  int count = 0;
  for(;;){
//    Serial.print("You heard: <");
    if(flag){
      count = 0;
      while(flag){
        if(message[count] == '\n') flag = 0;
        Serial.print(message[count]);
        count++;
      }
//      Serial.println(">");
      vTaskDelay(portTICK_PERIOD_MS);
    }
    else{
//      Serial.println("Nothing>");
      vTaskDelay(portTICK_PERIOD_MS);
    }
  }
}


void setup() {
  Serial.begin(115200);
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              vRecievingTask,  // Function to be called
              "to_listen",   // Name of task
              1024,         // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,         // Parameter to pass to function
              1,            // Task priority (0 to configMAX_PRIORITIES - 1)
              NULL,         // Task handle
              app_cpu);     // Run on one core for demo purposes (ESP32 only)
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              vSendingTask,  // Function to be called
              "to_listen",   // Name of task
              1024,         // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,         // Parameter to pass to function
              1,            // Task priority (0 to configMAX_PRIORITIES - 1)
              NULL,         // Task handle
              app_cpu);

}



void loop() {
//  Message_t message;
//  void *address = &message;
//  Serial.println((int)address);
//  Serial.println(message.sentence_size);
}
