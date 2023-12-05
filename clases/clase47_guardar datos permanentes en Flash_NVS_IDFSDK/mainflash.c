
// #define  LEDC_HIGH_SPEED_MODE  0
#define CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY 1


#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// #include "esp_now.h"
// #include "esp_wifi.h"
// #include "esp_netif.h"
// #include "esp_mac.h"
// #include "esp_event.h"

#include "nvs.h"
#include "nvs_flash.h"

//#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"

//#include "driver/spi_master.h"
// #include "freertos/timers.h"
// #include "rgb_led.h"
// #include <sdkconfig.h>
// #include "driver/ledc.h" 
// #include "driver/adc.h"
// #include "driver/uart.h"
// #include "freertos/queue.h"




#define LED_STRIP 8
#define LED_STRIP_MAX_LEDS 1

#define ledR 2
#define ledG 3
#define ledB 9

uint8_t led_color=0;

static const char *TAG = "NVS_TEST";
led_strip_handle_t led_strip;
nvs_handle_t app_nvs_handle;

static esp_err_t init_led_strip(void){

    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_STRIP,             // The GPIO that connected to the LED strip's data line
        .max_leds = LED_STRIP_MAX_LEDS,                            // The number of LEDs in the strip,
        .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
        .led_model = LED_MODEL_WS2812,            // LED strip model
        .flags.invert_out = false,                // whether to invert the output signal (useful when your hardware has a level inverter)
    };

    led_strip_spi_config_t spi_config = {
        .clk_src = SPI_CLK_SRC_DEFAULT, // different clock source can lead to different power consumption
        .flags.with_dma = true,         // Using DMA can improve performance and help drive more LEDs
        .spi_bus = SPI2_HOST,           // SPI bus ID
    };
    ESP_ERROR_CHECK(led_strip_new_spi_device(&strip_config, &spi_config, &led_strip));
    return ESP_OK;
}
esp_err_t init_led(void);

static esp_err_t init_nvs(){
    esp_err_t error;
    nvs_flash_init();
    error=nvs_open(TAG,NVS_READWRITE,&app_nvs_handle);
    if(error!=ESP_OK){
        ESP_LOGE(TAG,"Error opening NVS");
    }else{
        ESP_LOGI(TAG,"init_nvs completed");
    }

    return error;

}

static esp_err_t read_nvs(char *key, uint8_t *value){
    esp_err_t error;
    error=nvs_get_u8(app_nvs_handle,key,value);
    if(error!=ESP_OK){
        ESP_LOGE(TAG,"Error reading NVS");
    }else{
        ESP_LOGI(TAG,"value read= %u",* value);
    }

    return error;

}

static esp_err_t write_nvs(char *key, uint8_t value){
    esp_err_t error;
    error=nvs_set_u8(app_nvs_handle,key,value);
    if(error!=ESP_OK){
        ESP_LOGE(TAG,"Error writing NVS");
    }else{
        ESP_LOGI(TAG,"value write= %u",value);
    }

    return error;

}


void app_main(void)
{
    char *key="color";
    ESP_ERROR_CHECK(init_led_strip());
    ESP_ERROR_CHECK(init_nvs());

    read_nvs(key,&led_color);
    while(1){
        switch (led_color)
        {
        case 0:
            led_strip_set_pixel(led_strip,0,255,0,0);//red
            break;
        case 1:
            led_strip_set_pixel(led_strip,0,0,255,0);//green
            break;
        case 2:
            led_strip_set_pixel(led_strip,0,0,0,255);//blue
            break;
        case 3:
            led_strip_set_pixel(led_strip,0,255,255,0);//yellow
            break;
        case 4:
            led_strip_set_pixel(led_strip,0,255,0,255);//purple
            break;
        case 5:
            led_strip_set_pixel(led_strip,0,0,255,255);//red
            break;
        case 6:
            led_strip_set_pixel(led_strip,0,255,255,255);//white
            break;
        
        default:
            break;
        }
        led_strip_refresh(led_strip);
        led_color++;
        if(led_color>6)led_color=0;
        vTaskDelay(pdMS_TO_TICKS(1000));
        write_nvs(key,led_color);
    }
    

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
