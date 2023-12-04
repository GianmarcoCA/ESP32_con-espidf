

#define CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY 1
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/timers.h"
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
TimerHandle_t xTimers;
int interval=1000;
int timerId=1;
uint8_t a=1;

static void set_rgb_led(void){
    pStrip_a=led_strip_init(0,RGB_LED,MAX_LEDS);
    pStrip_a->clear(pStrip_a,100);
}
esp_err_t blink_led(void);
esp_err_t set_timer(void);

void vTimerCallback(TimerHandle_t pxTimer){
    ESP_LOGI(tag,"Event was called from timer");
    blink_led();
}
void app_main(void)
{

    set_rgb_led();
    set_timer();

}
esp_err_t blink_led(void){
        a^=1;

        ESP_LOGI(tag, "LED YELLOW");
        //printf("Led level: %u\n", led_level);
        (a==1)?(pStrip_a->set_pixel(pStrip_a,0,10,10,0)):(pStrip_a->set_pixel(pStrip_a,0,0,0,0));
        pStrip_a->refresh(pStrip_a,100);

/*         ESP_LOGI(tag, "LED GREEN");
        pStrip_a->set_pixel(pStrip_a,0,0,255,255);
        pStrip_a->refresh(pStrip_a,100);
        vTaskDelay(pdMS_TO_TICKS(500));

        ESP_LOGI(tag, "LED BLUE");
        pStrip_a->set_pixel(pStrip_a,0,255,0,255);
        pStrip_a->refresh(pStrip_a,100);
        vTaskDelay(pdMS_TO_TICKS(500)); */
    return ESP_OK;
}
esp_err_t set_timer(void){
    ESP_LOGI(tag,"Timer init configuration");
    xTimers = xTimerCreate("Timer",
                           (pdMS_TO_TICKS(interval)),
                           pdTRUE,
                           (void *)timerId,
                           vTimerCallback);
    if(xTimers==NULL){
        ESP_LOGE(tag,"The timer was not created");
    }else{
        if(xTimerStart(xTimers,0)!=pdPASS){
            ESP_LOGE(tag,"The timer could not be set into the Active state");
        }
    }

    return ESP_OK;
}

