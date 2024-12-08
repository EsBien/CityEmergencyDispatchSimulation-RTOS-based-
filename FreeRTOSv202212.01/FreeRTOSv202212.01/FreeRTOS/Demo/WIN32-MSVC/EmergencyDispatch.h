#pragma once

#ifndef CITY_EMERGENCY_DISPATCH
#define CITY_EMERGENCY_DISPATCH

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

#include "DepartmentManagment.h"


// Event timer handle
extern TimerHandle_t xEventTimer;

// Task function
void vPoliceForwardTask(void* param);
void vAmbulanceForwardTask(void* param);
void vFireForwardTask(void* param);
void vCovidForwardTask(void* param);

// Eevent timer
void vEventTimerCallback(TimerHandle_t xTimer);

// Function to set priority based on event code
int getEventPriority(int eventCode);

//dynamically assigns a priority and a descriptive message to a new EmergencyEvent_t
void setNewEventMessage(EmergencyEvent_t* newEvent);

#endif CITY_EMERGENCY_DISPATCH //CITY_EMERGENCY_DISPATCH