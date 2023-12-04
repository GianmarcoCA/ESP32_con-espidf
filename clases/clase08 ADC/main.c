
// #define  LEDC_HIGH_SPEED_MODE  0
// #define CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY 1


#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/timers.h"
#include "rgb_led.h"
#include <sdkconfig.h>
#include "driver/ledc.h" 
#include "driver/adc.h"

#define LEDR 8
#define LEDG 3
#define LEDB 9
#define STACK_SIZE 1024*2



const char* tag= "PWM";

TimerHandle_t xTimers;
int interval=50;
int timerId=1;
int adc_val=0;

esp_err_t init_led(void);
esp_err_t blink_led(void);
esp_err_t set_timer(void);
esp_err_t set_adc(void);

void vTimerCallback(TimerHandle_t pxTimer){
    blink_led();
    adc_val=adc1_get_raw(ADC1_CHANNEL_0);
    int adc_case=adc_val/1000;//max=4094
    ESP_LOGI(tag,"ADC VAL:%i",adc_val);

    switch (adc_case)
    {
    case 0:
        gpio_set_level(LEDR, 0);
        gpio_set_level(LEDG, 0);
        gpio_set_level(LEDB, 0);
        break;
    case 1:
        gpio_set_level(LEDR, 1);
        gpio_set_level(LEDG, 0);
        gpio_set_level(LEDB, 0);
        break;
    case 2:
        gpio_set_level(LEDR, 1);
        gpio_set_level(LEDG, 1);
        gpio_set_level(LEDB, 0);
        break;
    case 3:
    case 4:
        gpio_set_level(LEDR, 1);
        gpio_set_level(LEDG, 1);
        gpio_set_level(LEDB, 1);
        break;
    default:
        break;
    }
}
void app_main(void)
{

    init_led();
    set_timer();

}
esp_err_t blink_led(void){
    // led_level=!led_level;
    // gpio_set_level(led1, GPIO_MODE_OUTPUT);

    return ESP_OK;
}
esp_err_t init_led(void){
    gpio_reset_pin(LEDR);
    gpio_set_direction(LEDR,GPIO_MODE_OUTPUT);
    gpio_reset_pin(LEDG);
    gpio_set_direction(LEDG,GPIO_MODE_OUTPUT);
    gpio_reset_pin(LEDB);
    gpio_set_direction(LEDB,GPIO_MODE_OUTPUT);
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
esp_err_t set_adc(void){

    adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_11);
    adc1_config_width(ADC_WIDTH_BIT_12);

    return ESP_OK;
}