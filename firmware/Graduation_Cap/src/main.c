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

#define TOTAL_LEDS 20

/**
 * @brief Enumeration of the led current state
 * 
 */
typedef enum led_state_t{
    LOW,  /**< GPIO is 0  or logic low */
    HIGH, /**< GPIO is 1 or logic high */
}led_state_t;

/**
 * @brief Structure that will represent the pickaxe in the
 *        graduation cap
 * 
 */
typedef struct PickAxe{
    uint led[TOTAL_LEDS];           /**< LED array */
    led_state_t state[TOTAL_LEDS];  /**< State array */
}PickAxe;

/**
 * @brief Structure that will represent the ADXL335 sensor that will
 *        store the analog readings of all the axis.
 * 
 */
typedef struct Accelerometer{
    uint x_axis; /**< X axis */
    uint y_axis; /**< Y axis */
    uint z_axis; /**< Z axis */
}Accelerometer;

/**
 * @brief Structure that will represent the Bolt 
 *        placed in the gradauation cap
 * 
 */
typedef struct Bolt{
    uint led; /**< Bolt gpio */
    led_state_t state; /**< Bolt gpio state */
}Bolt;

/**
 * @brief Update LED state to HIGH or LOW: @see gpio_get(uint gpio)
 * 
 * @param gpio GPIO pin that will be read 
 * @return led_state_t 
 */
led_state_t updateState(uint gpio);

/**
 * @brief Toggle the Bolt gpio and update it state
 * 
 * @param bolt pass by reference bolt
 */
void bolt_toggle(Bolt *bolt);


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

/**
 * @brief Bolt task that will toggle every 500ms
 * 
 * @param param pass reference argument to the task
 */
void bolt_task(void *param){
    //initialize bolt to GPIO 20, and OFF
    Bolt bolt = {.led = 20, .state = LOW};
    // initialize GPIO
    gpio_init(bolt.led);
    // set the GPIO direction
    gpio_set_dir(bolt.led, GPIO_OUT);

    while(1){
        bolt_toggle(&bolt); //toggle the bolt LED
        vTaskDelay(500/portTICK_PERIOD_MS); // 1 sec delay
    }
}

int main()
{
    static const uint num = 25; //store in static memory
    stdio_init_all(); //initialize stdio for USB/UART
    TaskHandle_t ledTaskHandle = NULL;
    TaskHandle_t boltTaskHandle = NULL;

    xTaskCreate(led_task, "LED_Task", 1024,  (void * const)&num, 1, &ledTaskHandle);
    xTaskCreate(bolt_task, "Bolt Task", 1024, NULL, 2, &boltTaskHandle);
    //start scheduler
    vTaskStartScheduler();
    
    //should not reach this line
    while(1){};
}

led_state_t updateState(uint gpio){
    uint val = gpio_get(gpio);
    return val ? HIGH : LOW; 
}

void bolt_toggle(Bolt *bolt){

    if(bolt->state){
        gpio_put(bolt->led, 0); //turn off
    }else{
        gpio_put(bolt->led, 1); // turn on
    }
    //update state
    bolt->state = updateState(bolt->led);

    return;
}