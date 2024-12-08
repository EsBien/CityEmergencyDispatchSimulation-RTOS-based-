
#include "DepartmentManagment.h"
#include "Logging.h"
#include "queue.h"
#include <string.h>

// Semaphore declarations
SemaphoreHandle_t xPoliceSemaphore;
SemaphoreHandle_t xAmbulanceSemaphore;
SemaphoreHandle_t xFireSemaphore;
SemaphoreHandle_t xCovidSemaphore;


// Mutex declaration
SemaphoreHandle_t xPoliceMutex;
SemaphoreHandle_t xAmbulanceMutex;
SemaphoreHandle_t xFireMutex;
SemaphoreHandle_t xCovidMutex;

// Queue declaration
QueueHandle_t xPoliceQueue;
QueueHandle_t xAmbulanceQueue;
QueueHandle_t xFireQueue;
QueueHandle_t xCovidQueue;


void vPoliceTask(void* param) {
    LogEntry_t logEntry;
    TickType_t taskStartTime;
    TickType_t taskDuration;
    for (;;) {
        taskStartTime = xTaskGetTickCount(); // Record start time
        // Block until a notification is received from the dispatcher
        //ulTaskNotifyTake(pdTRUE, portMAX_DELAY);  // Wait indefinitely for a notification
        bool taskFaile = false;

        if (xSemaphoreTake(xPoliceMutex, portMAX_DELAY) == pdPASS) {

            if (xSemaphoreTake(xPoliceSemaphore, 0) == pdPASS) { // Blocks until a resource is available
                initLog(&logEntry, "Resource Acquired", POLICE_CODE, "Police", 0, "Primary Resource Acquired", taskStartTime);
                logEvent(logEntry);
            }
            else {

                taskFaile = true;
                initLog(&logEntry, "Resource Wait", POLICE_CODE, "Police", 0, "Waiting for Primary Resource", taskStartTime);
                logEvent(logEntry);
                xSemaphoreTake(xPoliceSemaphore, portMAX_DELAY);  // Wait for primary resource
                initLog(&logEntry, "Resource Acquired After Wait", POLICE_CODE, "Police", 0, "Primary Resource Acquired", xTaskGetTickCount());
                logEvent(logEntry);

            }

            taskFaile = checkTaskFailed(taskFaile, POLICE_CODE, "Police");


            if (taskFaile) continue; //Skip resource release if task failed

            xSemaphoreGive(xPoliceSemaphore);  // Return primary resource
            initLog(&logEntry, "Resource Released", POLICE_CODE, "Police", 0, "Primary Resource Released", xTaskGetTickCount());
            logEvent(logEntry);

            xSemaphoreGive(xPoliceMutex);  // Release mutex after use
        }
    }
}

void vAmbulanceTask(void* param) {
    SemaphoreHandle_t xBorrowedSemaphore = NULL;
    bool taskFaile = false;
    LogEntry_t logEntry = { 0 };
    TickType_t taskStartTime;
    TickType_t taskDuration;
    for (;;) {

        taskStartTime = xTaskGetTickCount();
        if (xSemaphoreTake(xAmbulanceMutex, portMAX_DELAY) == pdPASS) {
            if (xSemaphoreTake(xAmbulanceSemaphore, 0) == pdPASS) {
                initLog(&logEntry, "Resource Acquired", AMBULANCE_CODE, "Ambulance", 0, "Primary Resource Acquired", taskStartTime);
                logEvent(logEntry);
            }
            else {
                // onlu ambulance can borrow
                xBorrowedSemaphore = AmbulanceBorrowResource(PRIORITY_HIGH);
                if (xBorrowedSemaphore) { // is not null
                    initLog(&logEntry, "Resource Borrowed", AMBULANCE_CODE, "Ambulance", 0, "Borrowed Resource Acquired succefully", taskStartTime);
                    logEvent(logEntry);
                }
                else {
                    taskFaile = true;
                    initLog(&logEntry, "Resource Wait", AMBULANCE_CODE, "Ambulance", 0, "No resources available, waiting...", taskStartTime);
                    logEvent(logEntry);
                    xSemaphoreTake(xAmbulanceSemaphore, portMAX_DELAY);  // Wait for primary resource
                    initLog(&logEntry, "Resource Acquired After Wait", AMBULANCE_CODE, "Ambulance", 0, "Primary Resource Acquired", xTaskGetTickCount());
                    logEvent(logEntry);
                }
            }

            taskFaile = checkTaskFailed(taskFaile, AMBULANCE_CODE, "Ambulance");

            if (taskFaile) {
                continue;
            }//Skip resource release if task failed

            // Release resource back to the appropriate semaphore
            if (xBorrowedSemaphore) {
                xSemaphoreGive(xBorrowedSemaphore);  // Return borrowed resource
                xBorrowedSemaphore = NULL;           // Reset tracking variable
                initLog(&logEntry, "Released resource", AMBULANCE_CODE, "Ambulance", 0, "Released borrowed resource", xTaskGetTickCount());
                logEvent(logEntry);
            }
            else {
                xSemaphoreGive(xAmbulanceSemaphore);  // Return primary resource
                initLog(&logEntry, "Primary Released resource", AMBULANCE_CODE, "Ambulance", 0, "Released Primary resource", xTaskGetTickCount());
                logEvent(logEntry);
            }
            xSemaphoreGive(xAmbulanceMutex);  // Release mutex after use
        }
    }
}

void vFireTask(void* param) {
    LogEntry_t logEntry = { 0 };
    TickType_t taskStartTime;
    TickType_t taskDuration;
    bool taskFaile = false;
    for (;;) {
        taskStartTime = xTaskGetTickCount();
        if (xSemaphoreTake(xFireMutex, portMAX_DELAY) == pdPASS) {
            if (xSemaphoreTake(xFireSemaphore, 0) == pdPASS) {
                initLog(&logEntry, "Resource Acquired", FIRE_CODE, "Fire", 0, "Primary Resource Acquired", taskStartTime);
                logEvent(logEntry);
            }
            else {
                taskFaile = true;
                initLog(&logEntry, "Resource Wait", FIRE_CODE, "fire", 0, "No resources available, waiting...", taskStartTime);
                logEvent(logEntry);
                xSemaphoreTake(xFireSemaphore, portMAX_DELAY);  // Wait for primary resource
                initLog(&logEntry, "Resource Acquired After Wait", FIRE_CODE, "Fire", 0, "Primary Resource Acquired", xTaskGetTickCount());
                logEvent(logEntry);
            }


            taskFaile = checkTaskFailed(taskFaile, FIRE_CODE, "Fire");

            if (taskFaile) continue; //Skip resource release if task failed

            xSemaphoreGive(xFireSemaphore);  // Return primary resource
            initLog(&logEntry, "Primary Released resource", FIRE_CODE, "Fire", 0, "Released Primary resource", xTaskGetTickCount());
            logEvent(logEntry);
            xSemaphoreGive(xFireMutex);  // Release mutex after use
        }
    }
}

void vCoranaTask(void* param) {
    bool taskFaile = false;
    LogEntry_t logEntry = { 0 };
    TickType_t taskStartTime;
    TickType_t taskDuration;
    for (;;) {
        taskStartTime = xTaskGetTickCount();
        if (xSemaphoreTake(xCovidMutex, portMAX_DELAY) == pdPASS) {
            if (xSemaphoreTake(xCovidSemaphore, 0) == pdPASS) {
                initLog(&logEntry, "Resource Acquired", CORONA_CODE, "Corana", 0, "Primary Resource Acquired", taskStartTime);
                logEvent(logEntry);
            }
            else {

                initLog(&logEntry, "Resource Wait", CORONA_CODE, "Corana", 0, "No resources available, waiting...", taskStartTime);
                logEvent(logEntry);
                xSemaphoreTake(xCovidSemaphore, portMAX_DELAY);  // Wait for primary resource
                initLog(&logEntry, "Resource Acquired After Wait", CORONA_CODE, "Corana", 0, "Primary Resource Acquired", xTaskGetTickCount());
                logEvent(logEntry);

            }

            taskFaile = checkTaskFailed(taskFaile, CORONA_CODE, "Covid");

            if (taskFaile) continue; //Skip resource release if task failed

            xSemaphoreGive(xCovidSemaphore);  // Return primary resource
            initLog(&logEntry, "Primary Released resource", CORONA_CODE, "Corana", 0, "Released Primary resource", xTaskGetTickCount());
            logEvent(logEntry);

            xSemaphoreGive(xCovidMutex);  // Release mutex after use
        }
    }
}

bool checkTaskFailed(bool taskFailed, int departmentCodde, const char* message) {

    LogEntry_t logEntry = { 0 };
    TickType_t taskDuration;
    EmergencyEvent_t requeueEvent;
    if (!taskFailed) {
        // Task execution if resources were acquired successfully
        int taskDurationMs = (rand() % 4) * 1000;
        taskDuration = pdMS_TO_TICKS(taskDurationMs);
        vTaskDelay(taskDuration);

        initLog(&logEntry, "Task Completed", departmentCodde, message, taskDurationMs, "Task Completed Successfully", xTaskGetTickCount());
        logEvent(logEntry);
        return false;
    }
    else {
        // If task failed, re-queue it or redirect for further handling
        requeueEvent.eventCode = departmentCodde;
        requeueEvent.priority = departmentCodde;
        snprintf(requeueEvent.message, sizeof(requeueEvent.message), message);
        switch (requeueEvent.eventCode) {
        case POLICE_CODE:
        {
            if (xQueueSendFromISR(xPoliceQueue, &requeueEvent, 0) != pdPASS) {
                // Log failure to re-queue the task

                initLog(&logEntry, "Task Re-queue Failed", departmentCodde, message, 0, "Police Unable to Re-queue, Task Dropped", xTaskGetTickCount());
                logEvent(logEntry);
            }
            else {
                initLog(&logEntry, "Task Re-queue", departmentCodde, message, 0, "Police Re-queue, succefully", xTaskGetTickCount());
                logEvent(logEntry);
            }
            break;
        }
        case AMBULANCE_CODE: {
            if (xQueueSendFromISR(xAmbulanceQueue, &requeueEvent, 0) != pdPASS) {
                // Log failure to re-queue the task
                initLog(&logEntry, "Task Re-queue Failed", departmentCodde, message, 0, "Ambulance Unable to Re-queue, Task Dropped", xTaskGetTickCount());
                logEvent(logEntry);
            }
            else {     
                initLog(&logEntry, "Task Re-queue", departmentCodde, message, 0, "Ambulance Re-queue, succefully", xTaskGetTickCount());
                logEvent(logEntry);
            }
            break;
        }
        case FIRE_CODE: {
            if (xQueueSendFromISR(xFireQueue, &requeueEvent, 0) != pdPASS) {//xQueueSendFromISR
                // Log failure to re-queue the task
                initLog(&logEntry, "Task Re-queue Failed", departmentCodde, message, 0, "Fire Unable to Re-queue, Task Dropped", xTaskGetTickCount());
                logEvent(logEntry);
            }
            else {
                initLog(&logEntry, "Task Re-queue", departmentCodde, message, 0, "Fire Re-queue, succefully", xTaskGetTickCount());
                logEvent(logEntry);
            } break;
        }
        case CORONA_CODE: {
            if (xQueueSendFromISR(xCovidQueue, &requeueEvent, 0) != pdPASS) {
                // Log failure to re-queue the task
                initLog(&logEntry, "Task Re-queue Failed", departmentCodde, message, 0, "Covid Unable to Re-queue, Task Dropped", xTaskGetTickCount());
                logEvent(logEntry);
            } 
            else {
                initLog(&logEntry, "Task Failed", departmentCodde, message, 0, "Covid Re-queue, succefully", xTaskGetTickCount());
                logEvent(logEntry);
            }break;
        }
        default:
            snprintf(requeueEvent.message, sizeof(requeueEvent.message), "Unknown event received");
            break;
        }

        return true;  // Skip resource release if task failed
    }

}

int isResourceAvailable(const SemaphoreHandle_t semaphore) {
    UBaseType_t count = uxSemaphoreGetCount(semaphore);
    return count > 0;
}

SemaphoreHandle_t AmbulanceBorrowResource(int requestingDepartmentPriority) {
    // Check other department resources in priority order
    if (requestingDepartmentPriority == PRIORITY_HIGH) {
        if (xSemaphoreTake(xFireSemaphore, 0) == pdPASS) {
            printf("Resource borrowed from Fire.\n");
            return xFireSemaphore;
        }
        else {
            xSemaphoreGive(xFireSemaphore);
        }

    }
    else {
        if (xSemaphoreTake(xPoliceSemaphore, 0) == pdPASS) {
            printf("Resource borrowed from Police.\n");
            return xPoliceSemaphore;
        }
        else {
            xSemaphoreGive(xPoliceSemaphore);
        }


        if (isResourceAvailable(xCovidSemaphore) && xSemaphoreTake(xCovidSemaphore, 0) == pdPASS) {

            printf("Resource borrowed from covid.\n");
            return xCovidSemaphore;
        }
        else {
            xSemaphoreGive(xCovidSemaphore);
        }
    }


    return NULL;  // No resources available to borrow
}
