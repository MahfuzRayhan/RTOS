#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static const uint8_t ledPin = LED_BUILTIN;
SemaphoreHandle_t mutexHandle1 = xSemaphoreCreateBinary();
SemaphoreHandle_t mutexHandle2 = xSemaphoreCreateMutex();


void vBlinkingTask(void *pvParameters){
//  xSemaphoreTake(mutexHandle1, 0);
  int blink_rate = *(int *)pvParameters; 
  xSemaphoreGive(mutexHandle1);
  Serial.print("Received: ");
  Serial.println(blink_rate);
  pinMode(ledPin, OUTPUT);
  for(;;){
    digitalWrite(ledPin, HIGH);
    vTaskDelay(blink_rate/(2*portTICK_PERIOD_MS));
    digitalWrite(ledPin, LOW);
    vTaskDelay(blink_rate/(2*portTICK_PERIOD_MS));
  }
}

void setup() {
  Serial.begin(115200);
  int blink_rate;
  while(Serial.available() <=0);
  blink_rate = Serial.parseInt();
  
  Serial.print("Sending: ");
  Serial.println(blink_rate);
  Serial.println("Starting Tasks");
  xTaskCreatePinnedToCore(
                          vBlinkingTask,                   // Task Definition
                          "Incrementing Task A",               // Task Name
                          1024,                                // Stack Size
                          (void *)&blink_rate,                 // Task paremeters
                          1,                                   // Task Priority
                          NULL,                                // Task Handle
                          app_cpu); 
  xSemaphoreTake(mutexHandle1, portMAX_DELAY);
  Serial.println("Done");
//  vTaskDelay(2000/portTICK_PERIOD_MS);
  
}

void loop() {
  
}
