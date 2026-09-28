/*
	A sample project of WWDG
    In this project, the WWDG will be feed by the interrupt service function of WWDG.
    The LED0 is on first and then will be off when turn into the main dead loop.
    The LED1 will be toggled once the WWDG is fed, so it will flash with a relatively high frequency.
*/

#include "stm32f1xx_hal.h"
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"

#include "led.h"
#include "key.h"

WWDG_HandleTypeDef g_wwdg_handle;   /* 窗口看门狗句柄 */

void wwdg_init(uint8_t prerscaler, uint8_t window, uint8_t counter);

int main(void)
{
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* 设置时钟, 72Mhz */
    delay_init(72);                         /* 延时初始化 */
    BSP_LED_Init();
    BSP_LED0_ON();
    delay_ms(300);
    wwdg_init(WWDG_PRESCALER_8, 0x5F, 0x7F);             /* 初始化窗口看门狗(这里把十六进制数(unsigned int)传参给uint8_t类型，产生截断警告，问题不大) */
    while (1){
        BSP_LED0_OFF();
    }
}

/**
 * @brief  窗口看门狗初始化
 * @param  prerscaler: 预分配系数
 * @param  window: 窗口值
 * @param  counter: 计数值
 * @note   窗口看门狗
 */
void wwdg_init(uint8_t prerscaler, uint8_t window, uint8_t counter){
    g_wwdg_handle.Instance = WWDG;
    g_wwdg_handle.Init.Prescaler = prerscaler;          /* 预分配系数 */
    g_wwdg_handle.Init.Window = window;                 /* 窗口值 */
    g_wwdg_handle.Init.Counter = counter;               /* 计数值 */
    g_wwdg_handle.Init.EWIMode = WWDG_EWI_ENABLE;       /* 窗口看门狗提前唤醒中断 */
    HAL_WWDG_Init(&g_wwdg_handle);                      /* 初始化并开始WWDG */
}

/**
 * @brief  开启窗口看门狗提前唤醒中断后，通过重写该底层初始化函数初始化WWDG中断
 */
void HAL_WWDG_MspInit(WWDG_HandleTypeDef* wwdgHandle){
    __HAL_RCC_WWDG_CLK_ENABLE();
    HAL_NVIC_SetPriority(WWDG_IRQn, 2, 3);
    HAL_NVIC_EnableIRQ(WWDG_IRQn);
}

/**
 * @brief  窗口看门狗中断服务函数
 */
void WWDG_IRQHandler(void){
    HAL_WWDG_IRQHandler(&g_wwdg_handle);
}

/**
 * @brief  窗口看门狗提前唤醒中断回调函数
 */
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg){
    /* 喂狗 */
    HAL_WWDG_Refresh(&g_wwdg_handle);
    /* 翻转LED1 */
    BSP_LED1_TOGGLE();
}
