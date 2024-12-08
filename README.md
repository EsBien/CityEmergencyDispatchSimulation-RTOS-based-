# City Emergency Dispatch Simulation (RTOS-based)
**A Real-Time Emergency Dispatch System Simulation**
This project simulates a City Emergency Dispatch System using FreeRTOS on a Windows environment. <br /> 
The system manages emergency events (Police, Fire, Ambulance, and COVID response) while adhering to real-time constraints.<br /> 
It demonstrates task scheduling, priority handling, resource synchronization, logging, and fault management.<br /> 

# Key Features:
Real-time Event Processing: Handles emergency events dynamically based on type and priority.<br /> 
Departmental Tasks: Each department processes events independently using task and queue mechanisms.<br /> 
Synchronization: Protects shared resources using semaphores and mutexes.<br /> 
Fault Management: Logs failures and ensures graceful handling of resource limitations.<br /> 
Dynamic Timing: Adjusts the event generation timer interval to simulate varying workloads.<br /> 

# Features
#🚨 Emergency Departments<br /> 
Handles emergencies.<br /> 
Uses dedicated resources protected by mutexes and semaphores.<br /> 
Logs all actions and re-queues tasks on failure.<br /> 

# 🔧 Task Management
Each department operates as a FreeRTOS task.<br /> 
Tasks wait for their respective resources using semaphores and mutexes.<br /> 
Failed tasks are re-queued into their department's task queue.<br /> 

# 📋 Logging
Every task logs its status, including:<br /> 
Resource acquisition and release.<br /> 
Task execution and completion.<br /> 
Errors and re-queuing events.<br /> 
