# ECNG3006 Lab 2 

## Versioning

The various versions are managed using releases/tags.

The tags are as follows:
- rrsched: Round Robin Scheduling without sleep on idle
- priorinherit: Priority Inheritance without sleep on idle
- priorinherit-alt: Priority Inheritance with modified priorities and no sleep on idle
- rrsched-sleep: Round Robin Scheduling with sleep on idle
- priorinherit-sleep: Priority Inheritance with sleep on idle
- priorinherit-alt-sleep: Priority Inheritance with modified priorities and sleep on idle

## P1
- /project/sdkconfig
  
  - This is generated by make menuconfig
  
- /project/build/include/sdkconfig.h
  
  - This is generated from sdkconfig

- /sdk/components/freertos/port/esp8266/include/freertos/FreeRTOSConfig.h

  - This specifies the behaviour of freertos on the esp8266

## P2
### Question 2 - Vary the assigned priorities for your tasks, and look at the difference in performance. Is it what you expected?
The changing the assigned priorities did not change the performance much. The usage by each task usually matched each
other. This is likely because each task relied on the execution before it to pass the semaphore, hence the priority was
usually passed down to the lowest task, which was then increased by the highest priority task. This was what I had
expected, given that the amount of execution time for each was expected to be low compared to the delay of 1 second 
each had.

### Question 3 - Does the use of the processor sleep affect the system performance? 
The use of processor sleep did1 affect performance. The impact was likely due to the overhead associated with sleeping 
and waking up the processor, but the sleep time chosen was small. The overall effect was that the percent usage for the
IDLE task increased (and the other tasks decreased). The percent usage for the IDLE task (without sleep) was normally 
27%, but with sleep it increased to around 33% (matching the LED on and off tasks).
