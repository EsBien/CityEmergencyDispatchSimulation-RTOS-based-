# City Emergency Dispatch Simulation (RTOS-based)
**A Real-Time Emergency Dispatch System Simulation** <br />
This project simulates a City Emergency Dispatch System using FreeRTOS on a Windows environment. <br /> 
The system manages emergency events (Police, Fire, Ambulance, and COVID response) while adhering to real-time constraints.<br /> 
It demonstrates task scheduling, priority handling, resource synchronization, logging, and fault management.<br /> 

# Project Overview
The system processes emergency events dynamically and dispatches them to the relevant departments.<br />
Each department manages limited resources and operates independently as FreeRTOS tasks.<br />
Synchronization mechanisms ensure safe access to shared resources while fault management guarantees system robustness even under heavy load.<br />

# Key Features
**🚨 Emergency Event Handling**<br />
- Dynamic Event Processing: Emergency calls are generated randomly with varying priorities.<br />
- Departmental Task Management: Police, Ambulance, Fire Department, and COVID Response departments handle events using dedicated task queues.<br />
- Real-time Event Processing: Handles emergency events dynamically based on type and priority.<br /> 
- Fault Management: Logs failures and ensures graceful handling of resource limitations.<br /> 
- Dynamic Timing: Adjusts the event generation timer interval to simulate varying workloads.<br /> 

# 🔧 Resource Management
Limited Resources: Each department operates within predefined resource constraints:<br />
- Ambulance: 4 vehicles<br />
- Police: 3 vehicles<br />
- Fire Department: 2 vehicles<br />
- COVID Response: 4 resources<br />
Resource Borrowing: Departments can borrow resources from others during overloads.<br />

# 🕒 Real-Time Scheduling
Events are processed based on priority using a dispatcher task.<br />
Tasks execute with real-time constraints, ensuring time-critical responses.<br />

# 📋 Logging and Fault Handling
Logs every action, including resource allocation, task execution, and failures.<br />
Tracks tasks that fail due to resource limitations and re-queues them for later processing.<br />

**Example log:** <br />
<span style="color: red">
[1000] Action: Timer Restarted | Event Code: -1 | Department: Event Timer | Duration: 3000 ticks | Outcome: New Interval Set <br />
[1000] Action: Event Received | Event Code: 2 | Department: A Dispatcher | Duration: 0 ticks | Outcome: Processing<br />
[1000] Action: Event Forwarded | Event Code: 2 | Department: Ambulance Department | Duration: 0 ticks | Outcome: Forwarding<br />
[1000] Action: Event Processed | Event Code: 2 | Department: Ambulance Department | Duration: 0 ticks | Outcome: Completed<br />
[1000] Action: Resource Acquired After Wait | Event Code: 2 | Department: Ambulance | Duration: 0 ticks | Outcome: Primary Resource Acquired<br />
[1000] Action: Event Received | Event Code: 2 | Department: A Dispatcher | Duration: 0 ticks | Outcome: Processing<br />
[1000] Action: Event Forwarded | Event Code: 2 | Department: Ambulance Department | Duration: 0 ticks | Outcome: Forwarding<br />
[1000] Action: Event Processed | Event Code: 2 | Department: Ambulance Department | Duration: 0 ticks | Outcome: Completed<br />
[1000] Action: Task Re-queue | Event Code: 2 | Department: Ambulance | Duration: 0 ticks | Outcome: Ambulance Re-queue, successfully
[1006] Action: Task Completed | Event Code: 3 | Department: Fire | Duration: 1000 ticks | Outcome: Task Completed Successfully<br />
[1006] Action: Primary Released resource | Event Code: 3 | Department: Fire | Duration: 0 ticks | Outcome: Released Primary resource<br />
[1006] Action: Resource Acquired | Event Code: 3 | Department: Fire | Duration: 0 ticks | Outcome: Primary Resource Acquired 
</span>


