#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static int rate_1 = 200;  // ms
static int num = 0;
static const int rate_2 = 500;  // ms

static const int led_pin = LED_BUILTIN;
unsigned long int a0 = 0, a1 = 0;
void blinkLEDTask1(void *pvParameter) {
  static int rate_1 = 200; 
  for(;;) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(rate_1/portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(rate_1/portTICK_PERIOD_MS);
    Serial.print("Blinking at: ");
    Serial.println(rate_1);
  }
}

// Our task: blink an LED at another rate
void blinkLEDTask2(void *pvParameter) {
  int temp = 0;
  for(;;) {
    num = 0;
    while(Serial.available()){
      temp = Serial.read()-48;
      if( (temp>=0) && (temp<10) )  num = num*10 + temp;
      else break;
    }
    if(num) rate_1 = num;
  }
}

void setup() {

  // Configure pin
  Serial.begin(115200);
  pinMode(led_pin, OUTPUT);

  // Task to run forever
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              blinkLEDTask1,  // Function to be called
              "Toggle 1",   // Name of task
              1024,         // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,         // Parameter to pass to function
              1,            // Task priority (0 to configMAX_PRIORITIES - 1)
              NULL,         // Task handle
              app_cpu);     // Run on one core for demo purposes (ESP32 only)

  // Task to run forever
//  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
//              blinkLEDTask2,  // Function to be called
//              "Toggle 2",   // Name of task
//              1024,         // Stack size (bytes in ESP32, words in FreeRTOS)
//              NULL,         // Parameter to pass to function
//              1,            // Task priority (0 to configMAX_PRIORITIES - 1)
//              NULL,         // Task handle
//              app_cpu);     // Run on one core for demo purposes (ESP32 only)

  // If this was vanilla FreeRTOS, you'd want to call vTaskStartScheduler() in
  // main after setting up your tasks.
}

void loop() {
  
}
