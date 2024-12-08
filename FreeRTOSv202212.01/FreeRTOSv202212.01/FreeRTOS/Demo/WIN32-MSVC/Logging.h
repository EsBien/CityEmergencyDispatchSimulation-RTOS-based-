#pragma once
#include "FreeRTOS.h"
#include <stdbool.h>

typedef struct {
    char action[50];         // Action description (e.g., "Event Received", "Task Started")
    int eventCode;           // Emergency event code
    char department[30];     // Name of the department handling the event
    int duration;            // Duration of the task in ticks
    char outcome[50];        // Outcome of the action (e.g., "Completed", "Borrowed Resource")
    TickType_t timestamp;    // Time at which the log was created ;
} LogEntry_t;

void logEvent(LogEntry_t logEntry);

void initLog(LogEntry_t * logEntry, const char* action, int eventCode, const char* department, int duration, const char* outcome, TickType_t timestamp);