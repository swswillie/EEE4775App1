#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// Set output to GPIO 2

#define LED_PIN GPIO_NUM_2 

// Blink Task: LED toggles every 250 ms
void blink_task(void *pvParameters) {
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    bool led_on = false;

    while (1) {
        gpio_set_level(LED_PIN, led_on ? 1 : 0);
        led_on = !led_on; // Toggle LED

        // Optional debug print
        // printf("LED %s\n", led_on ? "ON" : "OFF");

        vTaskDelay(pdMS_TO_TICKS(250)); // Wait between actions
    }
}

// Print Task: Runs every 10 seconds
void print_task(void *pvParameters) {
    while (1) {
        printf("System alive, time = %lu ms\n",
            (unsigned long)(xTaskGetTickCount() * portTICK_PERIOD_MS));

        vTaskDelay(pdMS_TO_TICKS(10000)); // Delay 10s
    }
}

void app_main(void) {
    // Set task stack size and priority
    xTaskCreate(blink_task, "BlinkTask", 2048, NULL, 1, NULL);
    xTaskCreate(print_task, "PrintTask", 2048, NULL, 1, NULL);
}
