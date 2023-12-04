
// #define  LEDC_HIGH_SPEED_MODE  0
#define CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY 1


#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_log.h"
// #include "freertos/timers.h"
// #include "rgb_led.h"
// #include <sdkconfig.h>
// #include "driver/ledc.h" 
// #include "driver/adc.h"
// #include "driver/uart.h"
// #include "freertos/queue.h"
#include "driver/i2c.h"




#define ledR 8
#define ledG 3
#define ledB 9
#define led 2
#define btn_isr 0
#define STACK_SIZE 1024*2
#define R_delay 10000
#define G_delay 1000
#define B_delay 2000


#define I2C_SLAVE_ADDR 0x68
#define TIMEOUT_MS 1000
#define DELAY_MS 1000
#define pinSDA  7    
#define pinSCL  6





static const char* TAG= "Main";


esp_err_t init_led(void);
static esp_err_t set_i2c(void);



void app_main(void)
{
    uint8_t  rx_data[12];
    uint8_t  command=0x00;
    init_led();
    ESP_ERROR_CHECK(set_i2c());
    while (1)
    {
        i2c_master_write_read_device(I2C_NUM_0,
                                     I2C_SLAVE_ADDR,
                                     &command,
                                     1,
                                     rx_data,
                                     12,
                                     pdMS_TO_TICKS(TIMEOUT_MS));

        ESP_LOG_BUFFER_HEX(TAG,rx_data,12);
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
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


    ESP_LOGI(TAG,"Init led completed");

    return ESP_OK;
}
static esp_err_t set_i2c(void){

    i2c_config_t i2c_config={};

    i2c_config.mode=I2C_MODE_MASTER;
    i2c_config.sda_io_num=pinSDA;
    i2c_config.scl_io_num=pinSCL;
    i2c_config.sda_pullup_en=true;
    i2c_config.scl_pullup_en=true;
    i2c_config.master.clk_speed=400000;
    i2c_config.clk_flags=0;
    
    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0,&i2c_config));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0,I2C_MODE_MASTER,0,0,ESP_INTR_FLAG_LEVEL1));
    return ESP_OK;
}