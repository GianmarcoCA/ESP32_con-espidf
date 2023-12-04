

#define CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY 1
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "rgb_led.h"
#include <sdkconfig.h>
#include "led_strip.h"

#define RGB_LED 8
#define MAX_LEDS 1
#define LEDG 3
#define LEDB 9
#define STACK_SIZE 1024*2
#define R_delay 400
#define G_delay 1000
#define B_delay 2000


const char* tag= "LED STRIP";
static led_strip_t *pStrip_a;

static void set_rgb_led(void){
    pStrip_a=led_strip_init(0,RGB_LED,MAX_LEDS);
    pStrip_a->clear(pStrip_a,100);
}
void app_main(void)
{
    set_rgb_led();
    while(1){
        ESP_LOGI(tag, "LED RED");
        pStrip_a->set_pixel(pStrip_a,0,255,255,0);
        pStrip_a->refresh(pStrip_a,100);
        vTaskDelay(pdMS_TO_TICKS(500));

        ESP_LOGI(tag, "LED GREEN");
        pStrip_a->set_pixel(pStrip_a,0,0,255,255);
        pStrip_a->refresh(pStrip_a,100);
        vTaskDelay(pdMS_TO_TICKS(500));

        ESP_LOGI(tag, "LED BLUE");
        pStrip_a->set_pixel(pStrip_a,0,255,0,255);
        pStrip_a->refresh(pStrip_a,100);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

