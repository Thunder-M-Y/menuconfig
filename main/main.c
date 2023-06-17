#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define LED_GPIO CONFIG_LED_GPIO_NUM   
#define KEY_GPIO CONFIG_KEY_GPIO_NUM

const char* TAG = "GPIO";

void IRAM_ATTR key_handler(void* args)
{
    gpio_set_level(LED_GPIO, !gpio_get_level(LED_GPIO));
}

void app_main(void)
{
#ifdef CONFIG_ENABLE_LED_CONFIG
    
    ESP_LOGE("LED configuration enabled\n");
    printf("LED GPIO: %d\n", LED_GPIO);
#endif

#ifdef CONFIG_ENABLE_KEY_CONFIG
    ESP_LOGE("Key configuration enabled\n");
    printf("Key GPIO: %d\n", KEY_GPIO);
#endif

    gpio_config_t gpio_cnf = {
        .mode = GPIO_MODE_INPUT_OUTPUT,
        .pin_bit_mask = 1ULL << LED_GPIO,
    };
    gpio_config(&gpio_cnf);

#ifdef CONFIG_LED_STAT_ON
    gpio_set_level(LED_GPIO, 1);
#else
    gpio_set_level(LED_GPIO, 0);
#endif

    gpio_config_t key_cnf = {
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = 1ULL << KEY_GPIO,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_POSEDGE,
    };
    gpio_config(&key_cnf);

    gpio_set_intr_type(KEY_GPIO, GPIO_INTR_POSEDGE);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(KEY_GPIO, key_handler, NULL);

    vTaskDelete(NULL);
}
