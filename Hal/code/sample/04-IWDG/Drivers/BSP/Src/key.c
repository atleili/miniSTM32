#include "key.h"

void BSP_KEY_Init(){
    BSP_KEY0_CLK_ENABLE();
    BSP_KEY1_CLK_ENABLE();
    BSP_WK_UP_CLK_ENABLE();

    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Pin = BSP_KEY0_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BSP_KEY0_PORT, &gpio_init_struct);
    gpio_init_struct.Pin = BSP_KEY1_PIN;
    HAL_GPIO_Init(BSP_KEY1_PORT, &gpio_init_struct);
    gpio_init_struct.Pin = BSP_WK_UP_PIN;
    gpio_init_struct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(BSP_WK_UP_PORT, &gpio_init_struct);
}
