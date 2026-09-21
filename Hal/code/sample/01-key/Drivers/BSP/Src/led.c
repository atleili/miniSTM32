#include "led.h"

void BSP_LED_Init(void){
    BSP_LED0_CLK_ENABLE();
    BSP_LED1_CLK_ENABLE();

    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Pin    =   BSP_LED0_PIN;
    gpio_init_struct.Mode   =   GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull   =   GPIO_PULLUP;
    gpio_init_struct.Speed  =   GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BSP_LED0_PORT, &gpio_init_struct);
    BSP_LED0_OFF();
    gpio_init_struct.Pin    =   BSP_LED1_PIN;
    HAL_GPIO_Init(BSP_LED1_PORT, &gpio_init_struct);
    BSP_LED1_OFF();
}
