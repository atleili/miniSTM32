#pragma once
#define BSP_MINISTM32 1
#if BSP_MINISTM32

/**
 * LED模块
 * 板载两个LED灯
 * LED0 - PA8
 * LED1 - PD2
 */
#define BSP_LED_ENABLE 1

/**
 * 独立按键模块
 * 板载三个独立按键
 * KEY0     - PC5   -   PULLDOWN
 * KEY1     - PA15  -   PULLDOWN
 * WK_UP    - PA0   -   PULLUP
 */
#define BSP_KEY_ENABLE 1

#endif
