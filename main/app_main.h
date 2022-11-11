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

// Configuration for FreeRTOS
#define configUSE_PREEMPTION   1
#define configUSE_TIME_SLICING 1
#define configUSE_MUTEXES      1

#define APP_TASK_STACK_SIZE            2048
#define APP_TASK_PRIORITY_TURN_OFF_LED 7
#define APP_TASK_PRIORITY_TURN_ON_LED  7
#define APP_TASK_PRIORITY_PRINT_STATUS 7

#define APP_500_MS        500
#define APP_ONE_SEC_TICKS (1000 / portTICK_RATE_MS)

#define APP_LED_PIN     2
#define APP_LED_PIN_SEL (1ULL << APP_LED_PIN)

#define APP_SEMAPHORE_TAKE_WAIT_TICK ((TickType_t)1)

#define APP_RUNTIME_STAT_BUFFER_SIZE 200

static void app_turn_on_led(void *arg);

static void app_turn_off_led(void *arg);

static void app_print_status(void *arg);

/**
 * @brief Launches three tasks, then exits. The tasks are app_turn_on_led,
 * app_turn_off_led and app_print_status.
 */
void appmain(void);

#endif
