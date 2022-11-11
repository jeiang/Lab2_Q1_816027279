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
static const char       *TAG_ACTIVE_DELAY = "Actively waiting...";
static SemaphoreHandle_t app_led_mutex    = NULL;
static char              app_runtime_stat_buffer[APP_RUNTIME_STAT_BUFFER_SIZE];

static void
app_active_delay(uint32_t ms)
{
    // Using xTaskGetTickCount to get estimate of current time
    uint32_t start_time     = xTaskGetTickCount();
    uint32_t current_time   = xTaskGetTickCount();
    uint32_t end_delay_time = (ms / portTICK_RATE_MS) + start_time;
    
    ESP_LOGD(TAG_ACTIVE_DELAY, "Actively waiting for %d ms...", ms);
    
    while (current_time < end_delay_time)
    {
        current_time = xTaskGetTickCount();
    }
}

static void
app_turn_on_led(void *arg)
{
    while (1)
    {
        // Should not happen
        if (app_led_mutex != NULL)
        {
            // Poll the semaphore
            while (xSemaphoreTake(app_led_mutex, APP_SEMAPHORE_TAKE_WAIT_TICK)
                   != pdTRUE)
            {
            }
            gpio_set_level(APP_LED_PIN, 1);
            xSemaphoreGive(app_led_mutex);
        }
        ESP_LOGI(TAG_TURN_ON_LED, "Starting active wait...");
        app_active_delay(APP_500_MS);
        ESP_LOGI(TAG_TURN_ON_LED, "Changing from active wait to task delay...");
        vTaskDelay(APP_ONE_SEC_TICKS);
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
            while (xSemaphoreTake(app_led_mutex, APP_SEMAPHORE_TAKE_WAIT_TICK)
                   != pdTRUE)
            {
            }
            gpio_set_level(APP_LED_PIN, 0);
            xSemaphoreGive(app_led_mutex);
        }
        ESP_LOGI(TAG_TURN_OFF_LED, "Starting active wait...");
        app_active_delay(APP_500_MS);
        ESP_LOGI(TAG_TURN_OFF_LED,
                 "Changing from active wait to task delay...");
        vTaskDelay(APP_ONE_SEC_TICKS);
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
            while (xSemaphoreTake(app_led_mutex, APP_SEMAPHORE_TAKE_WAIT_TICK)
                   != pdTRUE)
            {
            }
            bool led_is_on = gpio_get_level(APP_LED_PIN) == 0 ? false : true;
            xSemaphoreGive(app_led_mutex);
            if (led_is_on)
            {
                ESP_LOGI(TAG_PRINT_STATUS, "LED is currently on.");
            }
            else
            {
                ESP_LOGI(TAG_PRINT_STATUS, "LED is currently off.");
            }
        }

        vTaskDelay(APP_ONE_SEC_TICKS);
    }
}

void
app_main(void)
{
    // Initialize arrays.
    memset(app_runtime_stat_buffer, APP_RUNTIME_STAT_BUFFER_SIZE, sizeof(char));

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
