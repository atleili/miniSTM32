/*
	A sample project of key input with HAL lib
	Three key 
*/
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"

#include "led.h"
#include "key.h"

uint8_t key_scan(uint8_t mode);

#define KEY0_PRES 1     /* key0 按下 */
#define KEY1_PRES 2     /* key1 按下 */

int main(void)
{
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* 设置时钟, 72Mhz */
    delay_init(72);                         /* 延时初始化 */
    BSP_LED_Init();
    BSP_KEY_Init();
    
    uint8_t key;

    while(1)
    {
        key = key_scan(0);
        if (key){
            switch (key)
            {
            case KEY0_PRES: BSP_LED0_TOGGLE();
                break;
            case KEY1_PRES: BSP_LED1_TOGGLE();
                break;
            }
        }
        else {
            delay_ms(10);
        }
    }
}

/**
 * @brief 按键扫描函数
 * @note 根据该逻辑，优先级key1>key0
 * @param mode:0/1
 *      0, 不支持连续按压，当按键按下不放时，只看作一次点击
 *      1，支持连续按压，当按键按下不放时，看作不断的连续点击
 * @retval 按键键值：
 *          0：没有按键按下
 *          KEY0_PRES(1)：key0按下
 *          KEY1_PRES(2): key1按下
 */
uint8_t key_scan(uint8_t mode){
    static uint8_t key_up = 1;  /* 按键松开标志 */
    uint8_t keyval = 0;         /* 按键值 */
    if(mode) key_up = 1;
    if(key_up && (BSP_KEY0 == 0 || BSP_KEY1 == 0)){
        /* 软件滤波 */
        delay_ms(10);
        /* 有按键按下 */
        key_up = 0;
        if(BSP_KEY0 == 0) keyval = KEY0_PRES;
        if(BSP_KEY1 == 0) keyval = KEY1_PRES;
    }
    else if(BSP_KEY0 == 1 && BSP_KEY1 == 1){
        /* 没有按键按下 */
        key_up = 1;
    }
    return keyval;
}
