
// #define  LEDC_HIGH_SPEED_MODE  0
#define CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY 1


#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "driver/spi_master.h"
// #include "freertos/timers.h"
// #include "rgb_led.h"
// #include <sdkconfig.h>
// #include "driver/ledc.h" 
// #include "driver/adc.h"
// #include "driver/uart.h"
// #include "freertos/queue.h"





#define ledR 8
#define ledG 3
#define ledB 9

 

#define pinMOSI  7    
#define pinMISO  2
#define pinCLK  6
#define pinCS  10


#define DECODE_MODE_REG 0x9//0
#define INTENSITY_REG 0xA//7
#define SCAN_LIMIT_REG 0xB//7
#define SHUTDOWN_REG 0xC//1
#define DISPLAY_TEST_REG 0xF//0


static const char* TAG= "SPI MASTER";
spi_device_handle_t spi_device_handle;

static esp_err_t init_spi(void){
    spi_bus_config_t spi_bus_config={};
    spi_bus_config.mosi_io_num=pinMOSI;
    spi_bus_config.miso_io_num=pinMISO;
    spi_bus_config.sclk_io_num=pinCLK;
    spi_bus_config.quadwp_io_num=-1;
    spi_bus_config.quadhd_io_num=-1;
    spi_bus_config.max_transfer_sz=32;

    spi_device_interface_config_t spi_device_interface_config={};
    spi_device_interface_config.mode=0;
    spi_device_interface_config.duty_cycle_pos=128;
    spi_device_interface_config.clock_speed_hz=1000000;
    spi_device_interface_config.spics_io_num=pinCS;
    spi_device_interface_config.flags=SPI_DEVICE_HALFDUPLEX;
    spi_device_interface_config.queue_size=1;
    spi_device_interface_config.pre_cb=NULL;
    spi_device_interface_config.post_cb=NULL;

    spi_bus_initialize(SPI2_HOST,&spi_bus_config,SPI_DMA_CH_AUTO);
    spi_bus_add_device(SPI2_HOST,&spi_device_interface_config,&spi_device_handle);



    return ESP_OK;
}
static void spi_write(uint8_t reg, uint8_t value){
    uint8_t data[2]={reg,value};
    spi_transaction_t spi_transaction={
        .tx_buffer=data,
        .length=16
    };
    spi_device_transmit(spi_device_handle,&spi_transaction);
}
esp_err_t init_led(void);
static esp_err_t max7219_init(void){
    spi_write(DECODE_MODE_REG,0);
    spi_write(INTENSITY_REG,7);
    spi_write(SCAN_LIMIT_REG,7);
    spi_write(SHUTDOWN_REG,1);
    spi_write(DISPLAY_TEST_REG,0);

    return ESP_OK;

}
static void clear_max7219(void){
    for (size_t i = 0; i < 8; i++)
    {
        spi_write(i+1,0);
    }
    
}
void app_main(void)
{


    ESP_ERROR_CHECK(init_led());
    ESP_ERROR_CHECK(init_spi());
    ESP_ERROR_CHECK(max7219_init());
    clear_max7219();

    spi_write(1,0xFF);
    spi_write(2,0x81);
    spi_write(3,0x81);
    spi_write(4,0x99);
    spi_write(5,0x99);
    spi_write(6,0x81);
    spi_write(7,0x81);
    spi_write(8,0xFF);

    


}
    


esp_err_t init_led(void){
    gpio_reset_pin(ledR);
    gpio_set_direction(ledR,GPIO_MODE_OUTPUT);
    gpio_reset_pin(ledG);
    gpio_set_direction(ledG,GPIO_MODE_OUTPUT);
    gpio_reset_pin(ledB);
    gpio_set_direction(ledB,GPIO_MODE_OUTPUT);
 

    ESP_LOGI(TAG,"Init led completed");

    return ESP_OK;
}
