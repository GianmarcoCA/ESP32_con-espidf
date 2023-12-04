
#define  LEDC_HIGH_SPEED_MODE  0
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
#include "driver/ledc.h" 

#define LEDR 8
#define LEDG 3
#define LEDB 9
#define STACK_SIZE 1024*2



const char* tag= "PWM";

TimerHandle_t xTimers;
int interval=50;
int timerId=1;
int dutyR=0;
int dutyG=300;
int dutyB=600;

esp_err_t init_led(void);
esp_err_t blink_led(void);
esp_err_t set_timer(void);
esp_err_t set_pwm(void);
esp_err_t set_pwm_duty(void);

void vTimerCallback(TimerHandle_t pxTimer){
    dutyR+=10;
    if(dutyR>1023)dutyR=0;
    dutyG+=10;
    if(dutyG>1023)dutyG=0;
    dutyB+=10;
    if(dutyB>1023)dutyB=0;
    blink_led();
    set_pwm_duty();
}
void app_main(void)
{

    init_led();
    set_pwm();
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
esp_err_t set_pwm(void){
    ledc_channel_config_t channelConfigR={0};
    channelConfigR.gpio_num=LEDR;
    channelConfigR.speed_mode=LEDC_HIGH_SPEED_MODE;
    channelConfigR.channel=LEDC_CHANNEL_0;
    channelConfigR.intr_type=LEDC_INTR_DISABLE;
    channelConfigR.timer_sel=LEDC_TIMER_0;
    channelConfigR.duty=0;

    ledc_channel_config_t channelConfigG={0};
    channelConfigG.gpio_num=LEDG;
    channelConfigG.speed_mode=LEDC_HIGH_SPEED_MODE;
    channelConfigG.channel=LEDC_CHANNEL_1;
    channelConfigG.intr_type=LEDC_INTR_DISABLE;
    channelConfigG.timer_sel=LEDC_TIMER_0;
    channelConfigG.duty=0;

    ledc_channel_config_t channelConfigB={0};
    channelConfigB.gpio_num=LEDB;
    channelConfigB.speed_mode=LEDC_HIGH_SPEED_MODE;
    channelConfigB.channel=LEDC_CHANNEL_2;
    channelConfigB.intr_type=LEDC_INTR_DISABLE;
    channelConfigB.timer_sel=LEDC_TIMER_0;
    channelConfigB.duty=0;

    ledc_channel_config(&channelConfigR);
    ledc_channel_config(&channelConfigG);
    ledc_channel_config(&channelConfigB);

    ledc_timer_config_t timerConfig={0};
    timerConfig.speed_mode=LEDC_HIGH_SPEED_MODE;
    timerConfig.duty_resolution=LEDC_TIMER_10_BIT;
    timerConfig.timer_num=LEDC_TIMER_0;
    timerConfig.freq_hz=20000; //20KHz

    ledc_timer_config(&timerConfig);
    return ESP_OK;
}
esp_err_t set_pwm_duty(void){
    ledc_set_duty(LEDC_HIGH_SPEED_MODE,LEDC_CHANNEL_0,dutyR);
    ledc_set_duty(LEDC_HIGH_SPEED_MODE,LEDC_CHANNEL_1,dutyG);
    ledc_set_duty(LEDC_HIGH_SPEED_MODE,LEDC_CHANNEL_2,dutyB);

    ledc_update_duty(LEDC_HIGH_SPEED_MODE,LEDC_CHANNEL_0);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE,LEDC_CHANNEL_1);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE,LEDC_CHANNEL_2);

    return ESP_OK;
}
