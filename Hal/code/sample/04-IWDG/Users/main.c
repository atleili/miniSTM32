/*
	A sample project of IWDG
    In this sample KEY0 is used to feed the IWDG, if KEY0 is not pressed, the IWDG will be reset
    and the LED0 will be reset to off when MCU reset,after 1s, LED0 will be turned on. So, if the IWDG
    can't be feed, the LED0 will be in a flashing state.
*/
/**
 * @note STM32F1最多支持5路串口，其中3个USART和两个UART
 *      USART1时钟源来源于APB2，最高频率72Mhz，其他串口来源于APB1，最高36Mhz
 */
#include "stm32f1xx_hal.h"
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"

#include "led.h"
#include "key.h"

IWDG_HandleTypeDef g_iwdg_handle;   /* 独立看门狗句柄 */

void iwdg_init(uint8_t prer, uint16_t reload);
void iwdg_feed(void);

int main(void)
{
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* 设置时钟, 72Mhz */
    delay_init(72);                         /* 延时初始化 */
    BSP_LED_Init();
    BSP_KEY_Init();
    delay_ms(1000); /* LED0初始化时为熄灭状态，延时1s从而在看门狗不断溢出时可以表现为闪烁状态 */
    BSP_LED0_ON(); /* 点亮LED0 */
    iwdg_init(IWDG_PRESCALER_64, 625);   /* 初始化独立看门狗, 溢出时间 = 64 * 625 / 40kHz = 1s */
    while (1){
        /* 如果KEY0按下，则喂狗 */
        if(BSP_KEY0 == BSP_KEY0_PRESSED){
            iwdg_feed();
        }
        delay_ms(10);
    }
}

/**
 * @brief 初始化独立看门狗
 * @note IWDG时钟源来源于低速内部时钟(LSI)，频率40KHz
 * @param prer 预分频系数
 * @param reload 重装载值
 */
void iwdg_init(uint8_t prer, uint16_t reload){
    g_iwdg_handle.Instance = IWDG;
    g_iwdg_handle.Init.Prescaler = prer;
    g_iwdg_handle.Init.Reload    = reload;
    HAL_IWDG_Init(&g_iwdg_handle);
}

void iwdg_feed(void){
    HAL_IWDG_Refresh(&g_iwdg_handle);
}
