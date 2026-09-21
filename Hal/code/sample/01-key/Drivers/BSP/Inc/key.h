/**
 * @note miniSTM32 板载三个独立轻触开关，其中
 *          KEY0     - PC5   -   PULLDOWN
 *          KEY1     - PA15  -   PULLDOWN
 *          WK_UP    - PA0   -   PULLUP
 */
#ifndef __KEY_H
#define __KEY_H

#include "bsp_hal_config.h"

#if BSP_KEY_ENABLE

#include "stm32f1xx_hal.h"

#define BSP_KEY0_PORT               GPIOC
#define BSP_KEY0_PIN                GPIO_PIN_5

#define BSP_KEY1_PORT               GPIOA
#define BSP_KEY1_PIN                GPIO_PIN_15

#define BSP_WK_UP_PORT              GPIOA
#define BSP_WK_UP_PIN               GPIO_PIN_0

#define BSP_KEY0_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()
#define BSP_KEY1_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define BSP_WK_UP_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define BSP_KEY0                    HAL_GPIO_ReadPin(BSP_KEY0_PORT, BSP_KEY0_PIN)
#define BSP_KEY1                    HAL_GPIO_ReadPin(BSP_KEY1_PORT, BSP_KEY1_PIN)
#define BSP_WK_UP                   HAL_GPIO_ReadPin(BSP_WK_UP_PORT, BSP_WK_UP_PIN)

void BSP_KEY_Init(void);

#endif  /* __KEY_H */
#endif
