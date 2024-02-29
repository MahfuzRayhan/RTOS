#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

typedef struct Msg{
  char body[20];
  int len;
}Msg;

static const uint8_t max_count = 5; 
static const uint8_t initial_count = 0;
static SemaphoreHandle_t smphrHandle = xSemaphoreCreateCounting(max_count, initial_count);


void vReadingTask(void *pvParameters){
  Msg message = *(Msg *)pvParameters;
  xSemaphoreGive(smphrHandle);
  Serial.print(message.body);
  Serial.print(" ");
  Serial.println(message.len);
  vTaskDelay(10/portTICK_PERIOD_MS);
//  vTaskDelete(NULL);
}

void setup() {
  Serial.begin(115200);
  char text[20] = "Hello from my side";
  Msg message; 
  strcpy(message.body, text);
  message.len = strlen(text);
  Serial.println("Starting Tasks");
  for ( int i = initial_count; i<max_count; i++){ 
    char task_identifier[10]; 
    sprintf( task_identifier, "Task %d", i);
    xTaskCreatePinnedToCore(
                            vReadingTask,                        // Task Definit
                            task_identifier,                     // Task Name
                            2048,                                // Stack Size
                            (void *)&message,                    // Task paremeters
                            1,                                   // Task Priority
                            NULL,                                // Task Handle
                            app_cpu); 
  }
  for( int i=initial_count; i< max_count; i++) xSemaphoreTake(smphrHandle, 100);
  Serial.println("Done");
}

void loop() {
//  vTaskDelay(1000/portTICK_PERIOD_MS);
}
