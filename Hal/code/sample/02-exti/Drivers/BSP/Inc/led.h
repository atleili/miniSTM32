/**
 * @note miniSTM32板载两个用户LED：
 *          LED0    -  PA8  -   LOW-ON
 *          LED1    -  PD2  -   LOW-ON
 */
#ifndef __LED_H
#define __LED_H

#include "bsp_hal_config.h"

#if BSP_LED_ENABLE

#include "stm32f1xx_hal.h"

#define BSP_LED0_PORT           GPIOA
#define BSP_LED0_PIN            GPIO_PIN_8

#define BSP_LED1_PORT           GPIOD
#define BSP_LED1_PIN            GPIO_PIN_2

#define BSP_LED0_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define BSP_LED1_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()

#define LED_ON                  GPIO_PIN_RESET
#define LED_OFF                 GPIO_PIN_SET

#define BSP_LED0_ON()           HAL_GPIO_WritePin(BSP_LED0_PORT, BSP_LED0_PIN, LED_ON)
#define BSP_LED0_OFF()          HAL_GPIO_WritePin(BSP_LED0_PORT, BSP_LED0_PIN, LED_OFF)
#define BSP_LED0_TOGGLE()       HAL_GPIO_TogglePin(BSP_LED0_PORT, BSP_LED0_PIN)

#define BSP_LED1_ON()           HAL_GPIO_WritePin(BSP_LED1_PORT, BSP_LED1_PIN, LED_ON)
#define BSP_LED1_OFF()          HAL_GPIO_WritePin(BSP_LED1_PORT, BSP_LED1_PIN, LED_OFF)
#define BSP_LED1_TOGGLE()       HAL_GPIO_TogglePin(BSP_LED1_PORT, BSP_LED1_PIN)

void BSP_LED_Init(void);

#endif  /* BSL_LED_ENABLE */
#endif  /* __LED_H */
