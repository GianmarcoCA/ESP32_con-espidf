

#define CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY 1
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "rgb_led.h"
#include <sdkconfig.h>


#define LEDR 8
#define LEDG 3
#define LEDB 9
#define STACK_SIZE 1024*2
#define R_delay 400
#define G_delay 1000
#define B_delay 2000


const char* tag= "Main";




void app_main(void)
{
    init_rgb_led(LEDR,LEDG,LEDB);
    while(1){
        set_rgb_red();
        vTaskDelay(pdMS_TO_TICKS(500));
        set_rgb_green();
        vTaskDelay(pdMS_TO_TICKS(500));
        set_rgb_blue();
        vTaskDelay(pdMS_TO_TICKS(500));
        set_rgb_off();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
 
}

