/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "app_main.h"

static const char       *TAG_MAIN         = "app_main";
static const char       *TAG_PRINT_STATUS = "Print Status";
static const char       *TAG_TURN_ON_LED  = "Turn LED on";
static const char       *TAG_TURN_OFF_LED = "Turn LED off";
static SemaphoreHandle_t app_led_mutex    = NULL;

static void
app_turn_on_led(void *arg)
{
    while (1)
    {
        // Should not happen
        if (app_led_mutex != NULL)
        {
            // Poll the semaphore
            if (xSemaphoreTake(app_led_mutex, (TickType_t)1))
            {
                gpio_set_level(APP_LED_PIN, 1);
                xSemaphoreGive(app_led_mutex);
            }
            else
            {
                // Unable to obtain semaphore if can reach here
                ESP_LOGW(TAG_TURN_ON_LED,
                         "Failed to obtain semaphore, despite polling.");
            }
        }
        ESP_LOGI(TAG_TURN_ON_LED, "Starting active wait...");
        // Using xTaskGetTickCount to get estimate of current time
        uint32_t start_time   = xTaskGetTickCount();
        uint32_t current_time = xTaskGetTickCount();
        while (current_time != start_time + APP_HALF_SEC_TICKS)
        {
            current_time = xTaskGetTickCount();
        }
        ESP_LOGI(TAG_TURN_ON_LED, "Changing from active wait to task delay...");
        vTaskDelay(APP_TASK_DELAY);
    }
}

static void
app_turn_off_led(void *arg)
{
    while (1)
    {
        // Should not happen
        if (app_led_mutex != NULL)
        {
            // Poll the semaphore
            if (xSemaphoreTake(app_led_mutex, (TickType_t)1))
            {
                gpio_set_level(APP_LED_PIN, 0);
                xSemaphoreGive(app_led_mutex);
            }
            else
            {
                // Unable to obtain semaphore if can reach her
                ESP_LOGW(TAG_TURN_OFF_LED,
                         "Failed to obtain semaphore, despite polling.");
            }
        }
        ESP_LOGI(TAG_TURN_ON_LED, "Starting active wait...");
        // Using xTaskGetTickCount to get estimate of current time
        uint32_t start_time   = xTaskGetTickCount();
        uint32_t current_time = xTaskGetTickCount();
        while (current_time != start_time + APP_HALF_SEC_TICKS)
        {
            current_time = xTaskGetTickCount();
        }
        ESP_LOGI(TAG_TURN_ON_LED, "Changing from active wait to task delay...");
        vTaskDelay(APP_TASK_DELAY);
    }
}

static void
app_print_status(void *arg)
{
    while (1)
    {
        // Should not happen
        if (app_led_mutex != NULL)
        {
            // Poll the semaphore
            if (xSemaphoreTake(app_led_mutex, (TickType_t)1))
            {
                bool led_is_on
                    = gpio_get_level(APP_LED_PIN) == 0 ? false : true;
                if (led_is_on)
                {
                    ESP_LOGI(TAG_PRINT_STATUS, "LED is currently on.");
                }
                else
                {
                    ESP_LOGI(TAG_PRINT_STATUS, "LED is currently on.");
                }
                xSemaphoreGive(app_led_mutex);
            }
            else
            {
                // Unable to obtain semaphore if can reach here
                ESP_LOGW(TAG_PRINT_STATUS,
                         "Failed to obtain semaphore, despite polling.");
            }
        }
        vTaskDelay(APP_TASK_DELAY);
    }
}

void
app_main(void)
{
    // Configure led output pin to output on gpio2
    gpio_config_t led_io_conf;
    led_io_conf.intr_type    = GPIO_INTR_DISABLE;
    led_io_conf.mode         = GPIO_MODE_OUTPUT;
    led_io_conf.pin_bit_mask = APP_LED_PIN_SEL;
    led_io_conf.pull_down_en = 0;
    led_io_conf.pull_up_en   = 0;
    gpio_config(&led_io_conf);

    app_led_mutex = xSemaphoreCreateMutex();

    if (app_led_mutex == NULL)
    {
        ESP_LOGE(TAG_MAIN, "Unable to create semaphore. Restarting...");
        esp_restart();
    }

    xTaskCreate(app_print_status,
                "app_print_status",
                APP_TASK_STACK_SIZE,
                NULL,
                APP_TASK_PRIORITY_PRINT_STATUS,
                NULL);
    xTaskCreate(app_turn_on_led,
                "app_turn_on_led",
                APP_TASK_STACK_SIZE,
                NULL,
                APP_TASK_PRIORITY_TURN_ON_LED,
                NULL);
    xTaskCreate(app_turn_off_led,
                "app_turn_off_led",
                APP_TASK_STACK_SIZE,
                NULL,
                APP_TASK_PRIORITY_TURN_OFF_LED,
                NULL);
}
