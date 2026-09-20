/*
	A sample project of key input with HAL lib
	Three key 
*/
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"

void led_init(void);                       /* LED初始化函数声明 */

int main(void)
{
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* 设置时钟, 72Mhz */
    delay_init(72);                         /* 延时初始化 */
    led_init();                             /* LED初始化 */
    
    while(1)
    { 
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);    /* PA8置1 */ 
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);  /* PD2置0 */ 
        delay_ms(500);
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);  /* PA8置0 */
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);    /* PD2置1 */
        delay_ms(500); 
    }
}

/**
 * @brief       初始化LED相关IO口, 并使能时钟
 * @param       无
 * @retval      无
 */
void led_init(void)
{
    GPIO_InitTypeDef gpio_initstruct;
    __HAL_RCC_GPIOA_CLK_ENABLE();                          /* IO口PA时钟使能 */
    __HAL_RCC_GPIOD_CLK_ENABLE();                          /* IO口PD时钟使能 */

    gpio_initstruct.Pin = GPIO_PIN_8;                      /* LED0引脚 */
    gpio_initstruct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_initstruct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_initstruct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(GPIOA, &gpio_initstruct);                /* 初始化LED0引脚 */

    gpio_initstruct.Pin = GPIO_PIN_2;                      /* LED1引脚 */
    HAL_GPIO_Init(GPIOD, &gpio_initstruct);                /* 初始化LED1引脚 */
}
