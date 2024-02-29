#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

int count = 0;


void vTaskA(void *pvParameter) {
  for(;;) {
    count = 0;
    Serial.println("From task A");
//    vTaskDelay(2/portTICK_PERIOD_MS);
  }
}

void vTaskB(void *pvParameter) {
  for(;;) {
    count = 0;
    Serial.println("From task B");
//    vTaskDelay(2/portTICK_PERIOD_MS);
  }
}



void setup() {
  Serial.begin(57600);
  vTaskDelay(1000/portTICK_PERIOD_MS);
  Serial.println("This should only be printed once");
  vTaskDelay(1000/portTICK_PERIOD_MS);
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              vTaskA,  // Function to be called
              "to_listen",   // Name of task
              1024,         // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,         // Parameter to pass to function
              2,            // Task priority (0 to configMAX_PRIORITIES - 1)
              NULL,         // Task handle
              app_cpu);     // Run on one core for demo purposes (ESP32 only)
//  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
//              vTaskB,  // Function to be called
//              "to_listen",   // Name of task
//              1024,         // Stack size (bytes in ESP32, words in FreeRTOS)
//              NULL,         // Parameter to pass to function
//              5,            // Task priority (0 to configMAX_PRIORITIES - 1)
//              NULL,         // Task handle
//              app_cpu);

}



void loop() {
  Serial.println(count++);
}
