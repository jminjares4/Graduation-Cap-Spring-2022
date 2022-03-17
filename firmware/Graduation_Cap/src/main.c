/**
 * @file main.c
 * @author Jesus Minjares (https://www.github.com/jminjares4)
 * @brief 
 * @version 0.1
 * @date 2022-03-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <task.h>
#include <stdio.h>
#include "pico/stdlib.h"

/**
 * @brief Led task to verify FreeRTOS 
 * 
 * @param param generic void * to pass as arguement
 */
void led_task(void *param){   

    const uint led = *(uint *)(param); //deference void *
    //initialize GPIO
    gpio_init(led);
    gpio_set_dir(led, GPIO_OUT);
    //loop
    while (true) {
        gpio_put(led, 1); //turn on
        vTaskDelay(1000/portTICK_PERIOD_MS);
        gpio_put(led, 0); //turn off
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

int main()
{
    static const uint num = 25; //store in static memory
    stdio_init_all(); //initialize stdio for USB/UART
    TaskHandle_t ledTaskHandle = NULL;

    xTaskCreate(led_task, "LED_Task", 1024,  (void * const)&num, 1, &ledTaskHandle);
    //start scheduler
    vTaskStartScheduler();
    
    //should not reach this line
    while(1){};
}
