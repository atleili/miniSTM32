/*
	A sample project of key input with HAL lib
	Three key 
*/
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"

void led_init(void);                       /* LED初始化函数声明 */
void key_init(void);                       /* key初始化函数声明 */
uint8_t key_scan(uint8_t mode);

#define LED0_TOOGLE HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8)
#define LED1_TOOGLE HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_2)

#define KEY0 HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5)        /* 读取 key0 */
#define KEY1 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15)       /* 读取 key1 */

#define KEY0_PRES 1     /* key0 按下 */
#define KEY1_PRES 2     /* key1 按下 */

int main(void)
{
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* 设置时钟, 72Mhz */
    delay_init(72);                         /* 延时初始化 */
    led_init();                             /* LED初始化 */
    key_init();                             /* key初始化 */
    
    uint8_t key;

    while(1)
    {
        key = key_scan(0);
        if (key){
            switch (key)
            {
            case KEY0_PRES: LED0_TOOGLE;
                break;
            case KEY1_PRES: LED1_TOOGLE;
                break;
            }
        }
        else {
            delay_ms(10);
        }
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

/**
  * @brief      初始化key相关IO口，并使能时钟
  * @param      无
  * @retval     无
  */
void key_init(void){
    GPIO_InitTypeDef gpio_initstruct;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /* key0 (按下为低电平) */
    gpio_initstruct.Pin = GPIO_PIN_5;
    gpio_initstruct.Mode = GPIO_MODE_INPUT;
    gpio_initstruct.Pull = GPIO_PULLUP;
    gpio_initstruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &gpio_initstruct);

    /* key1 (按下为低电平) */
    gpio_initstruct.Pin = GPIO_PIN_15;
    HAL_GPIO_Init(GPIOA, &gpio_initstruct);
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
    if(key_up && (KEY0 == 0 || KEY1 == 0)){
        /* 软件滤波 */
        delay_ms(10);
        /* 有按键按下 */
        key_up = 0;
        if(KEY0 == 0) keyval = KEY0_PRES;
        if(KEY1 == 0) keyval = KEY1_PRES;
    }
    else if(KEY0 == 1 && KEY1 == 1){
        /* 没有按键按下 */
        key_up = 1;
    }
    return keyval;
}
