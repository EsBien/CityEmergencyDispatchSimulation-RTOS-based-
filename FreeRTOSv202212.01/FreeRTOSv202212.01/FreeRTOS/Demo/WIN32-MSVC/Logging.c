
#include "Logging.h"
#include <stdio.h>
//#include <string.h>

//void logEvent(LogEntry_t logEntry) {
//#ifdef SIMULATION
//    printf("[%lu] Action: %s | Event Code: %d | Department: %s | Duration: %d ticks | Outcome: %s\n",
//        logEntry.timestamp, logEntry.action, logEntry.eventCode, logEntry.department, logEntry.duration, logEntry.outcome);
//#else
//    // Code for UART transmission in an embedded environment
//    // UART_SendFormatted("[%lu] Action: %s | Event Code: %d | Department: %s | Duration: %d ticks | Outcome: %s\n",
//    //                   logEntry.timestamp, logEntry.action, logEntry.eventCode, logEntry.department, logEntry.duration, logEntry.outcome);
//#endif
//}
void logEvent(LogEntry_t logEntry) {
    printf("[%lu] Action: %s | Event Code: %d | Department: %s | Duration: %d ticks | Outcome: %s \n",
                logEntry.timestamp, logEntry.action, logEntry.eventCode, logEntry.department, logEntry.duration, logEntry.outcome);
}

void initLog(LogEntry_t* logEntry, const char* action, int eventCode, const char* department, int duration, const char* outcome, TickType_t timestamp) {
    if (logEntry == NULL) 
        return; // Ensure logEntry pointer is valid

    strncpy(logEntry->action, action, sizeof(logEntry->action) - 1);
    logEntry->action[sizeof(logEntry->action) - 1] = '\0'; 

    logEntry->eventCode = eventCode;

    strncpy(logEntry->department, department, sizeof(logEntry->department) - 1);
    logEntry->department[sizeof(logEntry->department) - 1] = '\0';

    logEntry->duration = duration;

    strncpy(logEntry->outcome, outcome, sizeof(logEntry->outcome) - 1);
    logEntry->outcome[sizeof(logEntry->outcome) - 1] = '\0'; 

    logEntry->timestamp = timestamp;
    
}