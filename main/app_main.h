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

#define APP_TASK_STACK_SIZE 2048
#define APP_TASK_PRIORITY_TURN_OFF_LED 10
#define APP_TASK_PRIORITY_TURN_ON_LED 9
#define APP_TASK_PRIORITY_PRINT_STATUS 5

#define APP_HALF_SEC_TICKS (500 / portTICK_RATE_MS)
#define APP_TASK_DELAY (1000 / portTICK_RATE_MS)

#define APP_LED_PIN 2
#define APP_LED_PIN_SEL (1ULL << APP_LED_PIN)

static void app_turn_on_led(void *arg);

static void app_turn_off_led(void *arg);

static void app_print_status(void *arg);

/**
 * @brief Launches three tasks, then exits. The tasks are app_turn_on_led,
 * app_turn_off_led and app_print_status.
 */
void appmain(void);

#endif
