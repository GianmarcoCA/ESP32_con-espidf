
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
#include "driver/uart.h"




#define ledR 8
#define ledG 3
#define ledB 9
#define led 2
#define btn_isr 0
#define STACK_SIZE 1024*2
#define R_delay 10000
#define G_delay 1000
#define B_delay 2000

#define UART_NUM UART_NUM_1
#define pinTX 19
#define pinRX 18
#define BUF_SIZE 1024
#define TASK_MEMORY 1024*2


const char* tag= "Main";
char secuencia[4]={'R','G','B','N'};

esp_err_t init_led(void);
static void uart_task (void* pvParameters);
static void init_uart(void);



void app_main(void)
{

    init_led();
    init_uart();



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
static void uart_task (void* pvParameters){
    uint8_t *data=(uint8_t *)malloc(BUF_SIZE);
    uint8_t m=0;
    while(1){        
        vTaskDelay(pdMS_TO_TICKS(1000));
        if(m>3) m=0;
        uart_write_bytes(UART_NUM,(const char *)&secuencia[m++],1);
        bzero(data,BUF_SIZE);
        int len=uart_read_bytes(UART_NUM,data,BUF_SIZE,pdMS_TO_TICKS(100));
        if(len==0)
        {
            continue;
        }

        // uart_write_bytes(UART_NUM,(const char *)data,len);
        ESP_LOGI(tag, "Data received: %s",data);

        for (size_t i = 0; i < len; i++) //len-2 ,ya que la cadena de cadena de caracteres tiene \n y \r
        {
            char value=data[i];

            switch (value)
            {
            case 'R':
               gpio_set_level(ledR,1);
               gpio_set_level(ledG,0);
               gpio_set_level(ledB,0);
                break;
             case 'G':
               gpio_set_level(ledR,0);
               gpio_set_level(ledG,1);
               gpio_set_level(ledB,0);
                break;
             case 'B':
               gpio_set_level(ledR,0);
               gpio_set_level(ledG,0);
               gpio_set_level(ledB,1);
                break;
            
            default:
               gpio_set_level(ledR,0);
               gpio_set_level(ledG,0);
               gpio_set_level(ledB,0);
                break;
            }
        }
        
    }
}

static void init_uart(void){
    uart_config_t uartConf={
        .baud_rate=9600,
        .data_bits=UART_DATA_8_BITS,
        .parity=UART_PARITY_DISABLE,
        .stop_bits=UART_STOP_BITS_1,
        .flow_ctrl=UART_HW_FLOWCTRL_DISABLE,
        .source_clk=UART_SCLK_APB,
    };
    uart_param_config(UART_NUM,&uartConf);
    uart_set_pin(UART_NUM,pinTX,pinRX,UART_PIN_NO_CHANGE,UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM,BUF_SIZE,BUF_SIZE,0,NULL,0);
    xTaskCreate(uart_task,"uart_task",TASK_MEMORY,NULL,5,NULL);
    ESP_LOGI(tag, "Init uart completed");   
}