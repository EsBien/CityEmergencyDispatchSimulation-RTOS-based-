
#include "EmergencyDispatch.h"
#include "DepartmentManagment.h"
#include "Logging.h"
TimerHandle_t xEventTimer;


int main_final(void) {

    LogEntry_t logEntry;
    srand(time(NULL));
    // create a timer
    xEventTimer = xTimerCreate("Event Timer", pdMS_TO_TICKS(1000), pdTRUE, (void*)0, vEventTimerCallback);
    // start the timer
    xTimerStart(xEventTimer, 0);

    // create queue
    xPoliceQueue = xQueueCreate(QUEUE_DISPATCHER_LENGTH, sizeof(EmergencyEvent_t));
    xAmbulanceQueue = xQueueCreate(QUEUE_DISPATCHER_LENGTH, sizeof(EmergencyEvent_t));
    xFireQueue = xQueueCreate(QUEUE_DISPATCHER_LENGTH, sizeof(EmergencyEvent_t));
    xCovidQueue = xQueueCreate(QUEUE_DISPATCHER_LENGTH, sizeof(EmergencyEvent_t));
    if (xPoliceQueue == pdFAIL || xAmbulanceQueue == pdFAIL || xFireQueue == pdFAIL || xCovidQueue == pdFAIL) {
        initLog(&logEntry, "Failed to create", 0, "one of the queue failed to create", 0, "creation failed", 0);
        logEvent(logEntry);
    }

    // create semaphore depatments
    xAmbulanceSemaphore = xSemaphoreCreateCounting(AMBULANCE_SIZE, 0);
    xPoliceSemaphore = xSemaphoreCreateCounting(POLICE_SIZE , 0);
    xFireSemaphore = xSemaphoreCreateCounting(FIRE_SIZE, 0);
    xCovidSemaphore = xSemaphoreCreateCounting(CORONA_SIZE, 0);
    if (xPoliceSemaphore == pdFAIL || xAmbulanceSemaphore == pdFAIL || xFireSemaphore == pdFAIL || xCovidSemaphore == pdFAIL) {
        initLog(&logEntry, "Semaphore Creation Failed", 0,
            "Initialization", 0, "One or more semaphores failed to create", xTaskGetTickCount());
        logEvent(logEntry);
    }
    //create mutex
    xPoliceMutex = xSemaphoreCreateMutex();
    xAmbulanceMutex = xSemaphoreCreateMutex();
    xFireMutex = xSemaphoreCreateMutex();
    xCovidMutex = xSemaphoreCreateMutex();
    if (xPoliceMutex == pdFAIL || xAmbulanceMutex == pdFAIL || xFireMutex == pdFAIL || xCovidMutex == pdFAIL) {
        initLog(&logEntry, "Mutex Creation Failed", 0,
            "Initialization", 0, "One or more Mutex failed to create", xTaskGetTickCount());
        logEvent(logEntry);
    }
    // create task
    xTaskCreate(vPoliceForwardTask, "forward police Task", configMINIMAL_STACK_SIZE, NULL, PRIORITY_MEDIUM, NULL);
    xTaskCreate(vAmbulanceForwardTask, "forward Ambulance Task", configMINIMAL_STACK_SIZE, NULL, PRIORITY_HIGH, NULL);
    xTaskCreate(vFireForwardTask, "forward Fire Task", configMINIMAL_STACK_SIZE, NULL, PRIORITY_HIGH, NULL);
    xTaskCreate(vCovidForwardTask, "forward Covid Task", configMINIMAL_STACK_SIZE, NULL, PRIORITY_MEDIUM, NULL);

    xTaskCreate(vPoliceTask, "Police Task", configMINIMAL_STACK_SIZE, NULL, PRIORITY_LOW, NULL);
    xTaskCreate(vAmbulanceTask, "Ambulance Task", configMINIMAL_STACK_SIZE, NULL, PRIORITY_MEDIUM, NULL);
    xTaskCreate(vFireTask, "Fire Task", configMINIMAL_STACK_SIZE, NULL, PRIORITY_MEDIUM, NULL);
    xTaskCreate(vCoranaTask, "Corona Task", configMINIMAL_STACK_SIZE, NULL, PRIORITY_LOW, NULL);



    vTaskStartScheduler();

    for (;;); //should never reach her...
}