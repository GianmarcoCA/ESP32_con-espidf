
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
#define ledBoard 2
 


#define I2C_SLAVE_ADDR 0x32
#define RX_BUFFER_LEN 255
#define TX_BUFFER_LEN 255
#define TIMEOUT_MS 1000
#define DELAY_MS 1000
#define pinSDA  7    
#define pinSCL  6





static const char* TAG= "i2c-master";


esp_err_t init_led(void);
static esp_err_t set_i2c(void);
static void i2c_handle_task(void *pvParameters);
static esp_err_t create_i2c_handle_task(void);


void app_main(void)
{

    ESP_ERROR_CHECK(set_i2c());
    ESP_ERROR_CHECK(init_led());
    ESP_ERROR_CHECK(create_i2c_handle_task());
    


}
    


esp_err_t init_led(void){
    gpio_reset_pin(ledR);
    gpio_set_direction(ledR,GPIO_MODE_OUTPUT);
    gpio_reset_pin(ledG);
    gpio_set_direction(ledG,GPIO_MODE_OUTPUT);
    gpio_reset_pin(ledB);
    gpio_set_direction(ledB,GPIO_MODE_OUTPUT);
    gpio_reset_pin(ledBoard);
    gpio_set_direction(ledBoard,GPIO_MODE_OUTPUT);


    // gpio_set_level(ledR,1);
    // gpio_set_level(ledG,1);  solo para RGB
    // gpio_set_level(ledB,1);

    ESP_LOGI(TAG,"Init led completed");

    return ESP_OK;
}
static esp_err_t set_i2c(void){

    i2c_config_t i2c_config={};

    i2c_config.mode=I2C_MODE_SLAVE;
    i2c_config.sda_io_num=pinSDA;
    i2c_config.scl_io_num=pinSCL;
    i2c_config.sda_pullup_en=true;
    i2c_config.scl_pullup_en=true;
    i2c_config.slave.addr_10bit_en=0;
    i2c_config.slave.slave_addr=I2C_SLAVE_ADDR;
    i2c_config.clk_flags=0;
    
    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0,&i2c_config));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0,I2C_MODE_SLAVE,RX_BUFFER_LEN,TX_BUFFER_LEN,ESP_INTR_FLAG_LEVEL1));
    return ESP_OK;
}

static void i2c_handle_task(void *pvParameters){
    ESP_LOGI(TAG,"i2c_handle_task_start");
    uint8_t rx_data[RX_BUFFER_LEN]={0};
    uint8_t tx_data[32]="Hello from Slave!";
    while(1){
        vTaskDelay(pdMS_TO_TICKS(100));
        gpio_set_direction(ledBoard,1);
        int len=i2c_slave_read_buffer(I2C_NUM_0,&rx_data,RX_BUFFER_LEN,pdMS_TO_TICKS(100));
        if(len>0){
            gpio_set_direction(ledBoard,0); //apagar
            ESP_LOG_BUFFER_CHAR(TAG,rx_data,32);
            i2c_reset_rx_fifo(I2C_NUM_0);
            if(strncmp((const char *)rx_data,"LEDR",4)==0){
                gpio_set_level(ledR,1);
                gpio_set_level(ledG,0);  
                gpio_set_level(ledB,0);
            }
            if(strncmp((const char *)rx_data,"LEDG",4)==0){
                gpio_set_level(ledR,0);
                gpio_set_level(ledG,1);  
                gpio_set_level(ledB,0);
            }
            if(strncmp((const char *)rx_data,"LEDB",4)==0){
                gpio_set_level(ledR,0);
                gpio_set_level(ledG,1);  
                gpio_set_level(ledB,0);
            }

            i2c_slave_write_buffer(I2C_NUM_0,&tx_data,32,pdMS_TO_TICKS(100));
            bzero(rx_data,RX_BUFFER_LEN);
        }
    }   

}
static esp_err_t create_i2c_handle_task(void){
    xTaskCreate(i2c_handle_task,"i2c_handle_task",1024*4,NULL,1,NULL);
    return ESP_OK;
}