#pragma once
#ifndef DEPARTMENT_MANAGMENT
#define DEPARTMENT_MANAGMENT

#include "FreeRTOS.h"
#include "semphr.h"
#include <stdbool.h>

// Priority Levels
#define PRIORITY_LOW    1
#define PRIORITY_MEDIUM 2
#define PRIORITY_HIGH   3

// Department codes
#define POLICE_CODE 1
#define AMBULANCE_CODE 2
#define FIRE_CODE 3
#define CORONA_CODE 4
#define TOTAL_SIZE (POLICE_SIZE + AMBULANCE_SIZE + FIRE_SIZE + CORONA_SIZE)


// semaphore size
#define POLICE_SIZE 3
#define AMBULANCE_SIZE 4
#define FIRE_SIZE 2
#define CORONA_SIZE 4

#define MESSAGE_LEN 50

typedef struct {
    int eventCode;
    char message[MESSAGE_LEN];
    int priority;
}EmergencyEvent_t;

#define QUEUE_DISPATCHER_LENGTH 20

#define QUEUE_DISPATCHER_ITEM_SIZE sizeof( EmergencyEvent_t )

// Queue for dispatching events
extern QueueHandle_t xPoliceQueue;
extern QueueHandle_t xAmbulanceQueue;
extern QueueHandle_t xFireQueue;
extern QueueHandle_t xCovidQueue;

// Department task function prototypes
void vPoliceTask(void* param);
void vAmbulanceTask(void* param);
void vFireTask(void* param);
void vCoranaTask(void* param);

// Semaphores for each department's resources
extern SemaphoreHandle_t xPoliceSemaphore;
extern SemaphoreHandle_t xAmbulanceSemaphore;
extern SemaphoreHandle_t xFireSemaphore;
extern SemaphoreHandle_t xCovidSemaphore;
//extern SemaphoreHandle_t xBorrowedSemaphore;

//Mutec for each department resources
extern SemaphoreHandle_t xPoliceMutex;
extern SemaphoreHandle_t xAmbulanceMutex;
extern SemaphoreHandle_t xFireMutex;
extern SemaphoreHandle_t xCovidMutex;

// Utility function to check if a semaphore has any available resources
int isResourceAvailable(const  SemaphoreHandle_t semaphore);
// Function for a department to borrow a resource from another department
// only ambulance can borrow a resource from other departments
SemaphoreHandle_t AmbulanceBorrowResource(int requestingDepartmentPriority);
//check if a task failed if yes returns to the queue it came from
bool checkTaskFailed(bool taskFailed, int departmentCodde, const char* message);

#endif DEPARTMENT_MANAGMENT //DEPARTMENT_MANAGMENT
