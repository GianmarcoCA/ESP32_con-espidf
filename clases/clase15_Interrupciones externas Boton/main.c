
// #define  LEDC_HIGH_SPEED_MODE  0
#define CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY 1


#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_log.h"
// #include "freertos/timers.h"
// #include "rgb_led.h"
// #include <sdkconfig.h>
// #include "driver/ledc.h" 
// #include "driver/adc.h"




#define ledR 8
#define ledG 3
#define ledB 9
#define led 19
#define btn_isr 0
#define STACK_SIZE 1024*2
#define R_delay 10000
#define G_delay 1000
#define B_delay 2000



volatile uint8_t count=0;



const char* tag= "Main";

esp_err_t init_led(void);
esp_err_t init_irs(void);
void isr_handler(void* args);



void app_main(void)
{

    init_led();
    init_irs();
    while(1){
        ESP_LOGI(tag,"Count %u",count);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}

esp_err_t init_led(void){
    gpio_reset_pin(ledR);
    gpio_set_direction(ledR,GPIO_MODE_OUTPUT);
    gpio_reset_pin(ledG);
    gpio_set_direction(ledG,GPIO_MODE_OUTPUT);
    gpio_reset_pin(ledB);
    gpio_set_direction(ledB,GPIO_MODE_OUTPUT);
    gpio_reset_pin(led);
    gpio_set_direction(led,GPIO_MODE_OUTPUT);


    ESP_LOGI(tag,"Init led completed");

    return ESP_OK;
}

esp_err_t init_irs(void){

    gpio_config_t pGPIOConfig;
    pGPIOConfig.pin_bit_mask=(1ULL<<btn_isr);
    pGPIOConfig.mode=GPIO_MODE_DEF_INPUT;
    pGPIOConfig.pull_up_en=GPIO_PULLUP_ENABLE;
    pGPIOConfig.pull_down_en=GPIO_PULLDOWN_DISABLE;
    pGPIOConfig.intr_type=GPIO_INTR_NEGEDGE;

    gpio_config(&pGPIOConfig);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(btn_isr,isr_handler,NULL);

    ESP_LOGI(tag,"Init isr completed");
    return ESP_OK;

}

void isr_handler(void* args){

    count++;
    if(count>2){
        count=0;
    }
    switch (count)
    {
    case 0:
        gpio_set_level(led,1);
        break;
    case 1:
        gpio_set_level(led,0);
        break;
    case 2:
        gpio_set_level(led,1);
        break;  
    default:
        break;
    }
}