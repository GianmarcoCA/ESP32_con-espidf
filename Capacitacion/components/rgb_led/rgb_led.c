#include <stdio.h>
#include "rgb_led.h"

const char * TAG="RGB_LED";
uint8_t ledr=0;
uint8_t ledg=0;
uint8_t ledb=0;

esp_err_t init_rgb_led(uint8_t led_r,uint8_t led_g,uint8_t led_b){
    ledr=led_r;
    ledg=led_g;
    ledb=led_b;

    gpio_reset_pin(ledr);
    gpio_reset_pin(ledg);
    gpio_reset_pin(ledb);

    gpio_set_direction(ledr,GPIO_MODE_OUTPUT);
    gpio_set_direction(ledg,GPIO_MODE_OUTPUT);
    gpio_set_direction(ledb,GPIO_MODE_OUTPUT);

    ESP_LOGI(TAG, "init_rgb_led");
    return ESP_OK;
}

esp_err_t set_rgb_red(void) {
    gpio_set_level(ledr,1);
    gpio_set_level(ledg,0);
    gpio_set_level(ledb,0);
    ESP_LOGW(TAG, "R on");
     return ESP_OK;
}

esp_err_t set_rgb_green(void) {
    gpio_set_level(ledr,0);
    gpio_set_level(ledg,1);
    gpio_set_level(ledb,0);
    ESP_LOGW(TAG, "G on");
     return ESP_OK;
}
esp_err_t set_rgb_blue(void) {
    gpio_set_level(ledr,0);
    gpio_set_level(ledg,0);
    gpio_set_level(ledb,1);
    ESP_LOGW(TAG, "B on");
     return ESP_OK;
}

esp_err_t set_rgb_off(void) {
    gpio_set_level(ledr,0);
    gpio_set_level(ledg,0);
    gpio_set_level(ledb,0);
    ESP_LOGE(TAG, "ALL off");
     return ESP_OK;
}