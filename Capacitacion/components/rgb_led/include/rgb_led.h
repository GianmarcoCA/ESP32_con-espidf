#include "esp_err.h"
#include "driver/gpio.h"
#include "esp_log.h"

esp_err_t init_rgb_led(uint8_t led_r,uint8_t led_g,uint8_t led_b);
esp_err_t set_rgb_red();
esp_err_t set_rgb_green();
esp_err_t set_rgb_blue();
esp_err_t set_rgb_off();