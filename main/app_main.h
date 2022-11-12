/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#ifndef APP_MAIN_H
#define APP_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esp_log.h"
#include "esp_spi_flash.h"
#include "esp_system.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "driver/gpio.h"

#define configUSE_MUTEXES 1

// App Configuration
// Determines whether Round Robin or Priority Inheritance is used
//#define APP_ROUND_ROBIN_TASKS
// Determines whether the processor is put to sleep if the Idle Hook is used
#define APP_USE_IDLE_SLEEP
// Uses alternative priorities for priority inheritance
//#define APP_USE_ALT_PRIORITIES

// Declarations based on App Configuration
#ifdef APP_ROUND_ROBIN_TASKS
// Use Round Robin Scheduling
#define configUSE_PREEMPTION           1
#define configUSE_TIME_SLICING         1
#define APP_TASK_PRIORITY_TURN_OFF_LED 7
#define APP_TASK_PRIORITY_TURN_ON_LED  7
#define APP_TASK_PRIORITY_PRINT_STATUS 7
#else
// Use priority inheritance
#ifdef APP_USE_ALT_PRIORITIES
#define APP_TASK_PRIORITY_TURN_OFF_LED 10
#define APP_TASK_PRIORITY_TURN_ON_LED  5
#define APP_TASK_PRIORITY_PRINT_STATUS 1
#else
#define APP_TASK_PRIORITY_TURN_OFF_LED 4
#define APP_TASK_PRIORITY_TURN_ON_LED  9
#define APP_TASK_PRIORITY_PRINT_STATUS 10
#endif
#endif

#ifdef APP_USE_IDLE_SLEEP
#include "esp_sleep.h"
#define configUSE_IDLE_HOOK 1
#define APP_LIGHT_SLEEP_US  25000
#endif

// Misc Constants
#define APP_TASK_STACK_SIZE 2048

#define APP_500_MS        500
#define APP_ONE_SEC_TICKS (1000 / portTICK_RATE_MS)

#define APP_LED_PIN     2
#define APP_LED_PIN_SEL (1ULL << APP_LED_PIN)

#define APP_SEMAPHORE_TAKE_WAIT_TICK ((TickType_t)1)

#define APP_STATS_OUTPUT_BUFFER_SIZE 500

// Function Stubs

#ifdef APP_USE_IDLE_SLEEP
/**
 * @brief Is called when the IDLE task is triggered i.e. when the processor is
 * idling.
 */
void vApplicationIdleHook(void);
#endif

/**
 * @brief Turns on the LED on GPIO2
 */
static void app_turn_on_led(void *arg);

/**
 * @brief Turns off the LED on GPIO2
 */
static void app_turn_off_led(void *arg);

/**
 * @brief Prints the current status of the LED on GPIO2
 */
static void app_print_status(void *arg);

/**
 * @brief Launches three tasks, then exits. The tasks are app_turn_on_led,
 * app_turn_off_led and app_print_status.
 */
void appmain(void);

#endif
