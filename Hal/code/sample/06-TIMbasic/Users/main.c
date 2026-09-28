/*
	A sample project of basic Timer
    STM32F103 has 8 timers:
        2 of which are basic timers (TIM6, TIM7)
        4 of which are general-purpose timers (TIM2, TIM3, TIM4, TIM5)
        2 of which are advanced-control timers (TIM1, TIM8)
    This sample of basic timer uses TIM6 with interrupt of it, in which LED1 is toggled every 500ms,
    and in main loop, LED0 is toggled every 200ms.
*/

/*
    基本定时器包含TIM6和TIM7，具备：
        1个16位自动重装载寄存器（ARR）
        1个16位可编程预分频器（PSC）
        1个16位只能递增的计数器（CNT）
        可触发DAC同步电路、生成中断/DMA请求
*/

#include "stm32f1xx_hal.h"
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"

#include "led.h"
#include "key.h"

TIM_HandleTypeDef g_tim6_handle;

void tim6_init(uint16_t prescaler, uint16_t period);

int main(void)
{
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* 设置时钟, 72Mhz */
    delay_init(72);                         /* 延时初始化 */
    BSP_LED_Init();
    tim6_init(5000-1, 7200-1);                    /* 定时器初始化，定时500ms(7200*5000/72MHz) */
    while (1){
        BSP_LED0_TOGGLE();
        delay_ms(200);
    }
}

void tim6_init(uint16_t prescaler, uint16_t period){
    __HAL_RCC_TIM6_CLK_ENABLE();    /* 使能定时器时钟 */
    g_tim6_handle.Instance = TIM6;
    g_tim6_handle.Init.Prescaler = prescaler;                               /* 预分频 */
    g_tim6_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                    /* 递增计数器(基本定时器只有递增计数器) */
    g_tim6_handle.Init.Period = period;                                     /* 自动重装载值 */
    g_tim6_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;  /* 自动重装载预装载功能(不使用) */

    HAL_TIM_Base_Init(&g_tim6_handle);                                       /* 初始化定时器 */

    HAL_TIM_Base_Start_IT(&g_tim6_handle);                                   /* 启动定时器, 更新定时器中断(有自动重装载，不需要反复调用) */
}

/**
 * @brief   定时器底层硬件初始化函数，由HAL_TIM_Base_Init()调用的一个弱函数，用户通过重写该函数实现相关底层初始化
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim){
    if(htim->Instance == TIM6){
        HAL_NVIC_SetPriority(TIM6_IRQn, 1, 3);  /* 设置中断优先级 */
        HAL_NVIC_EnableIRQ(TIM6_IRQn);          /* 使能中断 */
    }
}

/* 中断服务函数 */
void TIM6_IRQHandler(void){
    /* 调用中断分发处理函数 */
    HAL_TIM_IRQHandler(&g_tim6_handle);
}

/* 定时器溢出更新中断回调函数，被中断分发处理函数触发 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance == TIM6){
        BSP_LED1_TOGGLE();
        // HAL_TIM_Base_Start_IT(htim);     /* 有自动重装载，不需要反复调用 */
    }
}
