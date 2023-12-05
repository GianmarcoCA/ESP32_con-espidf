
// #define  LEDC_HIGH_SPEED_MODE  0
#define CONFIG_FREERTOS_ENABLE_BACKWARD_COMPATIBILITY 1


#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_now.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_mac.h"
#include "esp_event.h"
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



#define ESP_CHANNEL 1
#define LED_STRIP 8
#define LED_STRIP_MAX_LEDS 1

#define ledR 2
#define ledG 3
#define ledB 9

static uint8_t peer_mac[ESP_NOW_ETH_ALEN]={0x58,0xcf,0x79,0x04,0x45,0x98};//tenemos que conectarnos al responder para obtener su mac address
static const char *TAG = "esp_now_init";
led_strip_handle_t led_strip;
static esp_err_t init_wifi(void){
    wifi_init_config_t wifi_init_config=WIFI_INIT_CONFIG_DEFAULT();

    esp_netif_init();
    esp_event_loop_create_default();
    nvs_flash_init();
    esp_wifi_init(&wifi_init_config);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_storage(WIFI_STORAGE_FLASH);
    esp_wifi_start();
    
    ESP_LOGI(TAG,"wifi init completed");
 
    return ESP_OK;
}

void recv_cb(const esp_now_recv_info_t * esp_now_info, const uint8_t *data, int data_len){

    ESP_LOGI(TAG,"Data received: " MACSTR "%s",MAC2STR(esp_now_info->src_addr),data);

}
 void send_cb(const uint8_t *mac_addr, esp_now_send_status_t status){
    if(status==ESP_NOW_SEND_SUCCESS){
        ESP_LOGI(TAG,"ESP_NOW_SEND_SUCCESS");
    }else{
        ESP_LOGW(TAG,"ESP_NOW_SEND_FAIL");
    }
 }
static esp_err_t init_esp_now(void){
    esp_now_init();
    esp_now_register_recv_cb(recv_cb);
    esp_now_register_send_cb(send_cb);

    ESP_LOGI(TAG,"esp now init completed");
    return ESP_OK;
}
static esp_err_t register_peer(uint8_t *peer_addr){
    esp_now_peer_info_t esp_now_peer_info={};
    memcpy(esp_now_peer_info.peer_addr,peer_addr, ESP_NOW_ETH_ALEN);
    esp_now_peer_info.channel=ESP_CHANNEL;
    esp_now_peer_info.ifidx=ESP_IF_WIFI_STA;

    esp_now_add_peer(&esp_now_peer_info);

    return ESP_OK;
}
static esp_err_t esp_now_send_data(const uint8_t *peer_addr,const uint8_t *data, uint8_t len){
    esp_now_send(peer_addr,data,len);
    return ESP_OK;
}

static esp_err_t init_led_strip(void){

    led_strip_handle_t led_strip;

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




void app_main(void)
{
    ESP_ERROR_CHECK(init_wifi());
    ESP_ERROR_CHECK(init_esp_now());
    ESP_ERROR_CHECK(register_peer(peer_mac));
    ESP_ERROR_CHECK(init_led_strip());

    uint8_t dataR[]="255|0|0";
    uint8_t dataG[]="0|255|0";
    uint8_t dataB[]="0|0|255";
    uint8_t count=0;
    while(1){
        count++;
        if(count>2) count=0;
        switch (count)
        {
        case 0:
            esp_now_send_data(peer_mac,dataR,32);
            led_strip_set_pixel(led_strip,0,255,0,0);

            break;
        case 1:
            esp_now_send_data(peer_mac,dataG,32);
            led_strip_set_pixel(led_strip,0,0,255,0);

            break;
        case 2:
            esp_now_send_data(peer_mac,dataB,32);
            led_strip_set_pixel(led_strip,0,0,0,255);

            break;



        default:
            break;
        }
        led_strip_refresh(led_strip);
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
 

    ESP_LOGI(TAG,"Init led completed");

    return ESP_OK;
}
