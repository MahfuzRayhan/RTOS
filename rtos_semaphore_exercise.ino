#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Settings
enum {BUF_SIZE = 5};                  // Size of buffer array
static const int num_prod_tasks = 5;  // Number of producer tasks
static const int num_cons_tasks = 2;  // Number of consumer tasks
static const int num_writes = 3;      // Num times each producer writes to buf

// Globals
static int buf[BUF_SIZE];                            // Shared buffer
static int head = 0;                                 // Writing index to buffer
static int tail = 0;                                 // Reading index to buffer
static SemaphoreHandle_t bin_sem;                    // Waits for parameter to be read
static SemaphoreHandle_t cnt_sem_filling;         // Counts the number of times producer prints
static SemaphoreHandle_t cnt_sem_emptying;         // Counts the number of times producer prints
static SemaphoreHandle_t mutex;                      // protects the serial buffer

//*****************************************************************************
// Tasks

// Producer: write a given number of times to shared buffer
void vProducerTask(void *pvParameters) {
  int num = *(int *)pvParameters;
  xSemaphoreGive(bin_sem);
  for (int i = 0; i < num_writes; i++) {
    if(xSemaphoreTake(cnt_sem_emptying, portMAX_DELAY) == pdTRUE){
        if(xSemaphoreGive(cnt_sem_filling) == pdTRUE){
          buf[head] = num;
          head = (head + 1) % BUF_SIZE;
          if(xSemaphoreTake(mutex, portMAX_DELAY)==pdTRUE){
            Serial.print("Placing "); 
            Serial.print(num);
            Serial.print(" at ");
            Serial.println(head);
            xSemaphoreGive(mutex);
          } else {  Serial.println("Couldn't get mutex"); }
        } else {  Serial.println("Filled up to limit");  }
    } else { Serial.println("No empty space"); } 
  }
  vTaskDelete(NULL);
}

// Consumer: continuously read from shared buffer
void vConsumerTask(void *pvParameters) {
  int val;
  char nem[20];
  sprintf(nem, "%s", pcTaskGetName(NULL));
  while (1) {
    if(xSemaphoreTake(cnt_sem_filling, portMAX_DELAY)==pdTRUE){
      if(xSemaphoreGive(cnt_sem_emptying)==pdTRUE){
        val = buf[tail];
        tail = (tail + 1) % BUF_SIZE;
        if(xSemaphoreTake(mutex, portMAX_DELAY)==pdTRUE){
          Serial.print("____________Removing "); 
          Serial.print(val);
          Serial.print(" from ");
          Serial.println(tail);
          xSemaphoreGive(mutex);
        }
        else { Serial.println("Couldn't get mutex"); }
      } else { Serial.println("Nothing to release"); }
    } else { Serial.println("Empty Buffer"); }
  }
}

//*****************************************************************************
// Main (runs as its own task with priority 1 on core 1)

void setup() {

  char task_name[12];
  
  // Configure Serial
  Serial.begin(115200);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Semaphore Alternate Solution---");

  // Create mutexes and semaphores before starting tasks
  bin_sem = xSemaphoreCreateBinary();
  cnt_sem_filling = xSemaphoreCreateCounting(BUF_SIZE, 0);
  cnt_sem_emptying = xSemaphoreCreateCounting(BUF_SIZE, BUF_SIZE);
  mutex =xSemaphoreCreateMutex();

//   Start producer tasks (wait for each to read argument)
  for (int i = 0; i < num_prod_tasks; i++) {
    sprintf(task_name, "Producer %i", i);
    xTaskCreatePinnedToCore(vProducerTask,
                            task_name,
                            1024,
                            (void *)&i,
                            2,
                            NULL,
                            app_cpu);
    xSemaphoreTake(bin_sem, portMAX_DELAY);
  }

  // Start consumer tasks
  for (int i = 0; i < num_cons_tasks; i++) {
    sprintf(task_name, "Consumer %i", i);
    xTaskCreatePinnedToCore(vConsumerTask,
                            task_name,
                            1024,
                            NULL,
                            2,
                            NULL,
                            app_cpu);
  }

  // Notify that all tasks have been created
  Serial.println("All tasks created");
}

void loop() {
  vTaskDelay(1000/portTICK_PERIOD_MS);
  }
