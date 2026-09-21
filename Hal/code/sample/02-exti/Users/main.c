/*
	A sample project of key input with HAL lib
	Three key 
*/
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"

#include "led.h"
#include "key.h"

#define KEY0_PRES 1     /* key0 按下 */
#define KEY1_PRES 2     /* key1 按下 */

void port_init(void);
void nvic_init(void);

int main(void)
{
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* 设置时钟, 72Mhz */
    delay_init(72);                         /* 延时初始化 */
    BSP_LED_Init();
    port_init();
    nvic_init();
    while (1)
    {
        delay_ms(1000);
    }
}

void port_init(void){
    BSP_KEY0_CLK_ENABLE();
    BSP_KEY1_CLK_ENABLE();

    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Pin = BSP_KEY0_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;   /* 下降沿触发中断 */
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BSP_KEY0_PORT, &gpio_init_struct);
    gpio_init_struct.Pin = BSP_KEY1_PIN;
    HAL_GPIO_Init(BSP_KEY1_PORT, &gpio_init_struct);
}

void nvic_init(void){
    /* 开启SYSCONFIG时钟 */
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    /*
        在HAL_Init()初始化中，已经设置了NVIC的优先级组
        HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2)
        即抢占优先级和子优先级各占2位，这里如果沿用就不需要再次设置优先级组
        数值越小，优先级越高
     */
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);   /* key0 使用EXTI9_5_IRQHandler函数， 抢占优先级2，子优先级0 */
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);   /* 使能中断线 */
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 1); /* key1 使用EXTI15_10_IRQHandler函数 抢占优先级2，子优先级1 */
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn); /* 使能中断线 */
}

/* 中断处理函数 */

void EXTI9_5_IRQHandler(void){
    /* 所有外部中断服务函数内调用该函数，作为外部中断共用入口函数，函数内进行中断标志位清零，并统一调用中断回调函数HAL_GPIO_EXTI_Callback() */
    HAL_GPIO_EXTI_IRQHandler(BSP_KEY0_PIN);
}

void EXTI15_10_IRQHandler(void){
    HAL_GPIO_EXTI_IRQHandler(BSP_KEY1_PIN);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    /* 正常项目中，尽量禁止在中断处理函数中使用延时函数 */
    delay_ms(20);
    switch (GPIO_Pin)
    {
    case BSP_KEY0_PIN:
        if(BSP_KEY0 == 0) BSP_LED0_TOGGLE();
        break;
    case BSP_KEY1_PIN:
        if(BSP_KEY1 == 0) BSP_LED1_TOGGLE();
    default:
        break;
    }
}
