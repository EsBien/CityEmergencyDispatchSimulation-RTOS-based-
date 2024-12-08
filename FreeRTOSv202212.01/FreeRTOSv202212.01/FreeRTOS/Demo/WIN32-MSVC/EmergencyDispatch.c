
#include "EmergencyDispatch.h"
#include "Logging.h"
#include <stdlib.h>
#include <string.h>
 // responsible for transmitting the transmitters

void setNewEventMessage(EmergencyEvent_t* newEvent) {
    LogEntry_t logEntry;
    newEvent->eventCode = (rand() % 4) + 1; // 1,2,3,4
    if (newEvent->eventCode < 0 || newEvent->eventCode >4) {
        printf("error event code");
    }
    newEvent->priority = getEventPriority(newEvent->eventCode);
    switch (newEvent->eventCode) {
    case POLICE_CODE:
    {
        snprintf(newEvent->message, sizeof(newEvent->message), "Police incident reported");
        break;
    }
    case AMBULANCE_CODE: {
        snprintf(newEvent->message, sizeof(newEvent->message), "AMBULANCE Medical emergency reported");
        break;
    }
    case FIRE_CODE: {
        snprintf(newEvent->message, sizeof(newEvent->message), "Fire emergency reported");
        break;
    }
    case CORONA_CODE:{
        snprintf(newEvent->message, sizeof(newEvent->message), "Corona emergency reported");
        break;
    }
    default:
        snprintf(newEvent->message, sizeof(newEvent->message), "Unknown event received");
        break;
    }
}

// generates a random event code (1-3) and sends it to the dispatcher queue
void vEventTimerCallback(TimerHandle_t xTimer) {

    EmergencyEvent_t newEvent = { .eventCode = -1,.message = {0} };
    LogEntry_t logEntry;
    setNewEventMessage(&newEvent);

    initLog(&logEntry, "Event Generated", newEvent.eventCode, "Timer Callback", 0, newEvent.message, xTaskGetTickCount());
    logEvent(logEntry);

    BaseType_t xHigherPriorityTaskWoken = pdFALSE; //do i need this

    switch (newEvent.eventCode) {
    case POLICE_CODE:
    {
        if (xQueueSendFromISR(xPoliceQueue, &newEvent, pdFALSE) == pdFAIL) {// Interrupt Service Routine
            initLog(&logEntry, "Police Queue Send Failed", newEvent.eventCode, "Dispatcher Queue", 0, "Send Failed", xTaskGetTickCount());
            logEvent(logEntry);
        }
        break;
    }
    case AMBULANCE_CODE: {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        if (xQueueSendFromISR(xAmbulanceQueue, &newEvent, xHigherPriorityTaskWoken) == pdFAIL) {// Interrupt Service Routine
            initLog(&logEntry, "AMBULANCE Queue Send Failed", newEvent.eventCode, "Dispatcher Queue", 0, "Send Failed", xTaskGetTickCount());
            logEvent(logEntry);
        }
        break;
    }
    case FIRE_CODE: {
        if (xQueueSendFromISR(xFireQueue, &newEvent, pdFALSE) == pdFAIL) {// Interrupt Service Routine
            initLog(&logEntry, "Fire Queue Send Failed", newEvent.eventCode, "Dispatcher Queue", 0, "Send Failed", xTaskGetTickCount());
            logEvent(logEntry);
        }
        break;
    }
    case CORONA_CODE: {
        if (xQueueSendFromISR(xCovidQueue, &newEvent, pdFALSE) == pdFAIL) {// Interrupt Service Routine
            initLog(&logEntry, "Corona Queue Send Failed", newEvent.eventCode, "Dispatcher Queue", 0, "Send Failed", xTaskGetTickCount());
            logEvent(logEntry);
        }
        break;
    }
    default:
        snprintf(newEvent.message, sizeof(newEvent.message), "Unknown event received");
        break;
    }
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken); //ensures timely response by immediately switching 

    // generates a random unterval 1-5
    xTimerStop(xTimer, 0);
    int newInterval = (rand() % 5 + 1) * 1000;
    xTimerChangePeriod(xTimer, pdMS_TO_TICKS(newInterval), 0);
    xTimerStart(xTimer, 0);

    // Log the timer restart with the new interval
    initLog(&logEntry, "Timer Restarted", -1, "Event Timer", newInterval, "New Interval Set", xTaskGetTickCount());
    logEvent(logEntry);

}


void vPoliceForwardTask(void* param) {
    EmergencyEvent_t receivedEvent;
    EmergencyEvent_t ptiotityQueueEvents[TOTAL_SIZE] = { 0 };
    LogEntry_t logEntry;
    int priorityEventCount = 0;

    for (;;) {

        if (xQueueReceive(xPoliceQueue, &receivedEvent, portMAX_DELAY) == pdPASS) {

            initLog(&logEntry, "Event Received", receivedEvent.eventCode, "Dispatcher", 0, "Processing", xTaskGetTickCount());
            logEvent(logEntry); // Log the receive event
      
            const char* departmentName = NULL;
            departmentName = "Police Department";
            if (xSemaphoreGive(xPoliceSemaphore) == pdFAIL) {
                initLog(&logEntry, "Failed Forwarded event", receivedEvent.eventCode, departmentName, 0, "Faile Forwarding", xTaskGetTickCount());
                logEvent(logEntry);
            }
            else {
                // Log forwarding the event to department
                initLog(&logEntry, "Event Forwarded", receivedEvent.eventCode, departmentName, 0, "Forwarding", xTaskGetTickCount());
                logEvent(logEntry);
            }
         
            initLog(&logEntry, "Event Processed", receivedEvent.eventCode, departmentName, 0, "Completed", xTaskGetTickCount());
            logEvent(logEntry);
        }
        else {
            //printf("Failed to receive event from DispatcherQueue\n");
            initLog(&logEntry, "Failed to Receive Event", 0, "police Dispatcher", 0, "Failed", xTaskGetTickCount());
            logEvent(logEntry);
        }

    }
}
void vAmbulanceForwardTask(void* param) {
    EmergencyEvent_t receivedEvent;
    EmergencyEvent_t ptiotityQueueEvents[TOTAL_SIZE] = { 0 };
    LogEntry_t logEntry;
    int priorityEventCount = 0;

    for (;;) {

        if (xQueueReceive(xAmbulanceQueue, &receivedEvent, portMAX_DELAY) == pdPASS) {

            initLog(&logEntry, "Event Received", receivedEvent.eventCode, "A Dispatcher", 0, "Processing", xTaskGetTickCount());
            logEvent(logEntry); // Log the receive event

            const char* departmentName = NULL;
            departmentName = "Ambulance Department";
            if (xSemaphoreGive(xAmbulanceSemaphore) == pdFAIL) {
                initLog(&logEntry, "Failed Forwarded event", receivedEvent.eventCode, departmentName, 0, "Faile Forwarding", xTaskGetTickCount());
                logEvent(logEntry);
            }
            else {
                // Log forwarding the event to department
                initLog(&logEntry, "Event Forwarded", receivedEvent.eventCode, departmentName, 0, "Forwarding", xTaskGetTickCount());
                logEvent(logEntry);
            }

            initLog(&logEntry, "Event Processed", receivedEvent.eventCode, departmentName, 0, "Completed", xTaskGetTickCount());
            logEvent(logEntry);
        }
        else {
            //printf("Failed to receive event from DispatcherQueue\n");
            initLog(&logEntry, "Failed to Receive Event", 0, "Ambulance Dispatcher", 0, "Failed", xTaskGetTickCount());
            logEvent(logEntry);
        }

    }
}
void vFireForwardTask(void* param) {
    EmergencyEvent_t receivedEvent;
    EmergencyEvent_t ptiotityQueueEvents[TOTAL_SIZE] = { 0 };
    LogEntry_t logEntry;
    int priorityEventCount = 0;

    for (;;) {

        if (xQueueReceive(xFireQueue, &receivedEvent, portMAX_DELAY) == pdPASS) {

            initLog(&logEntry, "Event Received", receivedEvent.eventCode, "fire", 0, "Processing", xTaskGetTickCount());
            logEvent(logEntry); // Log the receive event
     
            const char* departmentName = NULL;
            departmentName = "Fire Department";
            if (xSemaphoreGive(xFireSemaphore) == pdFAIL) {
                initLog(&logEntry, "Failed Forwarded event", receivedEvent.eventCode, departmentName, 0, "Faile Forwarding", xTaskGetTickCount());
                logEvent(logEntry);
            }
            else {
                // Log forwarding the event to department
                initLog(&logEntry, "Event Forwarded", receivedEvent.eventCode, departmentName, 0, "Forwarding", xTaskGetTickCount());
                logEvent(logEntry);
            }

            initLog(&logEntry, "Event Processed", receivedEvent.eventCode, departmentName, 0, "Completed", xTaskGetTickCount());
            logEvent(logEntry);
        }
        else {
            //printf("Failed to receive event from DispatcherQueue\n");
            initLog(&logEntry, "Failed to Receive Event", 0, "Fire Dispatcher", 0, "Failed", xTaskGetTickCount());
            logEvent(logEntry);
        }

    }
}
void vCovidForwardTask(void* param) {
    EmergencyEvent_t receivedEvent;
    EmergencyEvent_t ptiotityQueueEvents[TOTAL_SIZE] = { 0 };
    LogEntry_t logEntry;
    int priorityEventCount = 0;

    for (;;) {

        if (xQueueReceive(xCovidQueue, &receivedEvent, portMAX_DELAY) == pdPASS) {

            initLog(&logEntry, "Event Received", receivedEvent.eventCode, "Dispatcher", 0, "Processing", xTaskGetTickCount());
            logEvent(logEntry); // Log the receive event

            const char* departmentName = NULL;
            departmentName = "Covid Department";
            if (xSemaphoreGive(xCovidSemaphore) == pdFAIL) {
                initLog(&logEntry, "Failed Forwarded event", receivedEvent.eventCode, departmentName, 0, "Faile Forwarding", xTaskGetTickCount());
                logEvent(logEntry);
            }
            else {
                // Log forwarding the event to department
                initLog(&logEntry, "Event Forwarded", receivedEvent.eventCode, departmentName, 0, "Forwarding", xTaskGetTickCount());
                logEvent(logEntry);
            }

            initLog(&logEntry, "Event Processed", receivedEvent.eventCode, departmentName, 0, "Completed", xTaskGetTickCount());
            logEvent(logEntry);
        }
        else {
            initLog(&logEntry, "Failed to Receive Event", 0, "Covid Dispatcher", 0, "Failed", xTaskGetTickCount());
            logEvent(logEntry);
        }

    }
}


int getEventPriority(int eventCode) {
    switch (eventCode) {
    case POLICE_CODE: return PRIORITY_LOW;
    case AMBULANCE_CODE: return PRIORITY_HIGH;
    case FIRE_CODE: return PRIORITY_HIGH;
    case CORONA_CODE: return PRIORITY_LOW;
    default: return PRIORITY_LOW;
    }
}



