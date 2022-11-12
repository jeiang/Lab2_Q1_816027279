/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "app_main.h"
#include <string.h>

static const char       *APP_TAG_MAIN         = "app_main";
static const char       *APP_TAG_PRINT_STATUS = "Print Status";
static const char       *APP_TAG_TURN_ON_LED  = "Turn LED on";
static const char       *APP_TAG_TURN_OFF_LED = "Turn LED off";
static const char       *APP_TAG_ACTIVE_DELAY = "Actively waiting...";
static const char       *APP_TAG_IDLE_SLEEP   = "Idle Sleep Hook";
static SemaphoreHandle_t app_led_mutex        = NULL;
static char              app_stats_output_buffer[APP_STATS_OUTPUT_BUFFER_SIZE];

#ifdef APP_USE_IDLE_SLEEP
void
vApplicationIdleHook(void)
{
    esp_err_t ret_code;
    ret_code = esp_sleep_enable_timer_wakeup(APP_LIGHT_SLEEP_US);
    if (ret_code != ESP_OK)
    {
        ESP_LOGW(
            APP_TAG_IDLE_SLEEP,
            "Unable to set sleep duration. Error Code: %s. Not going to sleep.",
            esp_err_to_name(ret_code));
    }
    else
    {
        ret_code = esp_light_sleep_start();
        if (ret_code != ESP_OK)
        {
            ESP_LOGW(APP_TAG_IDLE_SLEEP,
                     "Failed to go to sleep. Error Code: %s",
                     esp_err_to_name(ret_code));
        }
        else
        {
            ESP_LOGD(APP_TAG_IDLE_SLEEP, "Slept successfully.");
        }
    }
}
#endif

static void
app_active_delay(uint32_t ms)
{
    // Using xTaskGetTickCount to get estimate of current time
    uint32_t start_time     = xTaskGetTickCount();
    uint32_t current_time   = xTaskGetTickCount();
    uint32_t end_delay_time = (ms / portTICK_RATE_MS) + start_time;

    ESP_LOGD(APP_TAG_ACTIVE_DELAY, "Actively waiting for %d ms...", ms);

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
            ESP_LOGI(APP_TAG_TURN_ON_LED, "Starting active wait...");
            app_active_delay(APP_500_MS);
            xSemaphoreGive(app_led_mutex);
        }
        ESP_LOGI(APP_TAG_TURN_ON_LED,
                 "Changing from active wait to task delay...");
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
            ESP_LOGI(APP_TAG_TURN_OFF_LED, "Starting active wait...");
            app_active_delay(APP_500_MS);
            xSemaphoreGive(app_led_mutex);
        }
        ESP_LOGI(APP_TAG_TURN_OFF_LED,
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
                ESP_LOGI(APP_TAG_PRINT_STATUS, "LED is currently on.");
            }
            else
            {
                ESP_LOGI(APP_TAG_PRINT_STATUS, "LED is currently off.");
            }
            vTaskGetRunTimeStats(app_stats_output_buffer);
            ESP_LOGI(APP_TAG_PRINT_STATUS,
                     "RUNTIME STATISTICS\nTask Name\tAbs. Time\t\tUsage %%\n%s",
                     app_stats_output_buffer);
        }

        vTaskDelay(APP_ONE_SEC_TICKS);
    }
}

void
app_main(void)
{
    // Initialize array
    memset(app_stats_output_buffer, APP_STATS_OUTPUT_BUFFER_SIZE, sizeof(char));

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
        ESP_LOGE(APP_TAG_MAIN, "Unable to create semaphore. Restarting...");
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
