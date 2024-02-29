#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

static TimerHandle_t one_shot_timer = NULL;
void myTimerCallbackFunction(TimerHandle_t xTimer){
  digitalWrite(LED_BUILTIN, LOW);
}

void vInactivityDetectionTask(void *pvParameters){
  pinMode(LED_BUILTIN, OUTPUT);
  while(1){
      if(Serial.available()){
        if(Serial.read()){
          if(xTimerIsTimerActive(one_shot_timer)==pdTRUE){
            xTimerReset(one_shot_timer, 0);
          }
          else{
            digitalWrite(LED_BUILTIN, HIGH);
            if(one_shot_timer == NULL) {  Serial.println("No timer created"); } 
            else{  xTimerStart(one_shot_timer, 0);  }
          }
        }
      }
  }
}

//*****************************************************************************
// Main (runs as its own task with priority 1 on core 1)

void setup() {
  Serial.begin(115200);
  vTaskDelay(100/portTICK_PERIOD_MS);
  Serial.println("------FreeRTOS Timer Demo--------");

  one_shot_timer = xTimerCreate(
                                "One-shot-timer",               // Timer Name
                                5000/portTICK_PERIOD_MS,        // Timer Period
                                pdFALSE,                        // Auto-reload settings
                                (void *)0,                      // Timer ID
                                myTimerCallbackFunction);       // Callback Function
  
  xTaskCreatePinnedToCore(
                            vInactivityDetectionTask,
                            "LookForCharactersInBuffer",
                            1024,
                            NULL,
                            1,
                            NULL,
                            app_cpu);
}

void loop() {
//  vTaskDelay(1000/portTICK_PERIOD_MS);
  }
