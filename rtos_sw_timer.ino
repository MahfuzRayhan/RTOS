#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

static TimerHandle_t one_shot_timer = NULL;
static TimerHandle_t auto_reload_timer = NULL;
void myTimerCallbackFunction(TimerHandle_t xTimer){
  if((uint32_t)pvTimerGetTimerID(xTimer)==0)  Serial.println("One Shot Timer Expired");
  if((uint32_t)pvTimerGetTimerID(xTimer)==1)  Serial.println("Auto Reload Timer Expired");
}


//*****************************************************************************
// Main (runs as its own task with priority 1 on core 1)

void setup() {
  Serial.begin(115200);
  vTaskDelay(100/portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("------FreeRTOS Timer Demo--------");

  one_shot_timer = xTimerCreate(
                                "One-shot-timer",               // Timer Name
                                2000/portTICK_PERIOD_MS,        // Timer Period
                                pdFALSE,                        // Auto-reload settings
                                (void *)0,                      // Timer ID
                                myTimerCallbackFunction);       // Callback Function

  auto_reload_timer = xTimerCreate(
                                "Auto-reload-timer",            // Timer Name
                                1000/portTICK_PERIOD_MS,        // Timer Period
                                pdTRUE,                         // Auto-reload settings
                                (void *)1,                      // Timer ID
                                myTimerCallbackFunction);       // Callback Function
  if ( one_shot_timer == NULL) {
    Serial.println("Could not create one of the timers...");
  } else {
    vTaskDelay(1000/portTICK_PERIOD_MS);
    Serial.println("Starting one shot timer...");
    xTimerStart(one_shot_timer, portMAX_DELAY);
  }
  if ( auto_reload_timer == NULL) {
    Serial.println("Could not create one of the timers...");
  } else {
    vTaskDelay(1000/portTICK_PERIOD_MS);
    Serial.println("Starting auto reload timer...");
    xTimerStart(auto_reload_timer, portMAX_DELAY);
  }
  vTaskDelete(NULL);
}

void loop() {
//  vTaskDelay(1000/portTICK_PERIOD_MS);
  }
