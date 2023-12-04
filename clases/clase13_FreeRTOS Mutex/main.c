#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"


#define ledR 8
#define ledG 3
#define ledB 9
#define STACK_SIZE 1024*2
#define R_delay 400
#define G_delay 1000
#define B_delay 2000


xSemaphoreHandle GlobalKey=0;

const char* tag= "Main";

esp_err_t init_led(void);
esp_err_t create_tasks(void);
void vTaskR(void * pvParameters);
void vTaskG(void * pvParameters);
esp_err_t shared_resource(int led);


void app_main(void)
{
    GlobalKey=xSemaphoreCreateMutex();
    init_led();
    create_tasks();
 
}

esp_err_t init_led(void){
    gpio_reset_pin(ledR);
    gpio_set_direction(ledR,GPIO_MODE_OUTPUT);
    gpio_reset_pin(ledG);
    gpio_set_direction(ledG,GPIO_MODE_OUTPUT);
    gpio_reset_pin(ledB);
    gpio_set_direction(ledB,GPIO_MODE_OUTPUT);
    return ESP_OK;
}

esp_err_t create_tasks(void){

    static uint8_t ucParameterToPass;
    TaskHandle_t xHandle = NULL;

    xTaskCreatePinnedToCore(vTaskR,
                "vTaskR",
                STACK_SIZE,
                &ucParameterToPass,
                1,
                &xHandle,
                0);

     xTaskCreatePinnedToCore(vTaskG,
                "vTaskG",
                STACK_SIZE,
                &ucParameterToPass,
                1,
                &xHandle,
                0);


    return ESP_OK;
}

void vTaskR(void * pvParameters){
    while(1){
        if(xSemaphoreTake(GlobalKey, pdMS_TO_TICKS(100))){
            ESP_LOGE(tag,"task R took the resource");
            shared_resource(ledR);
            xSemaphoreGive(GlobalKey);
        }
        vTaskDelay(pdMS_TO_TICKS(R_delay));
    }

}
void vTaskG(void * pvParameters){

    while(1){
        if(xSemaphoreTake(GlobalKey, pdMS_TO_TICKS(100))){
            ESP_LOGW(tag,"task G took the resource");
            shared_resource(ledG);
            xSemaphoreGive(GlobalKey);
        }
        vTaskDelay(pdMS_TO_TICKS(G_delay));
    }


}
esp_err_t shared_resource(int led){
    for (size_t i = 0; i < 8; i++)
    {
        vTaskDelay(pdMS_TO_TICKS(400));
        gpio_set_level(led,1);
        vTaskDelay(pdMS_TO_TICKS(400));
        gpio_set_level(led,0);
    }
    
    return ESP_OK;
}