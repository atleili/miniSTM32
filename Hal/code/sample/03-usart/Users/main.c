/*
	A sample project of usart1
    When use self written usart1, you should disable usart in Driver/SYSTEM/usart/usart.h
    This sample keeps the MCU listening the port of uart1, if any instruction with "\n" in the end, 
    it will be regard as a complete instruction and be sent back from uart1 without the "\n" end.
*/
/**
 * @note STM32F1最多支持5路串口，其中3个USART和两个UART
 *      USART1时钟源来源于APB2，最高频率72Mhz，其他串口来源于APB1，最高36Mhz
 */
#include "stm32f1xx_hal.h"
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include <stdbool.h>
#include <string.h>

#include "led.h"
#include "key.h"

#define RCC_UART1_CLK_ENABLE() do {__HAL_RCC_USART1_CLK_ENABLE(); __HAL_RCC_GPIOA_CLK_ENABLE();} while (0)  /* 使能串口和PA9、PA10引脚时钟 */

UART_HandleTypeDef g_huart1_handle;  /* UART1句柄 */

#define RX_BUF_SIZE 1 /* 接收缓冲区大小 */

uint8_t g_rx_buf[RX_BUF_SIZE];  /* 接收缓冲区 */

#define RX_LEN 200 /* 接收暂存区大小 */

uint8_t g_UART_rx_buf[RX_LEN];  /* 接收暂存区，接收来自接收缓冲区的数据，直到一个完整指令后一起处理 */

uint8_t g_rx_len = 0;  /* 接收暂存区当前接收到的数据长度 */

bool g_rx_flag = false;  /* 接收暂存区接收完成标志 */

#define TX_BUF_SIZE 200  /* 发送缓存区大小 */

uint8_t g_tx_buf[TX_BUF_SIZE];  /* 发送缓存区 */

void USART1_Init(void);
void UART1_Process(void);

int main(void)
{
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* 设置时钟, 72Mhz */
    delay_init(72);                         /* 延时初始化 */
    BSP_LED_Init();
    USART1_Init();
    while (1){
        UART1_Process();
        delay_ms(100);
        // /* 不断发送测试确认状态 */
        // HAL_UART_Transmit(&g_huart1_handle, (uint8_t*)"hello\r\n", strlen("hello\r\n"), 0xffff);
    }
}

void USART1_Init(void){
    g_huart1_handle.Instance = USART1;  /* 串口1 */
    g_huart1_handle.Init.BaudRate = 115200;                     /* 波特率 */
    g_huart1_handle.Init.WordLength = UART_WORDLENGTH_8B;       /* 数据位 */
    g_huart1_handle.Init.StopBits = UART_STOPBITS_1;            /* 停止位 */
    g_huart1_handle.Init.Parity = UART_PARITY_NONE;             /* 校验位 */
    g_huart1_handle.Init.Mode = UART_MODE_TX_RX;                /* 全双工 */
    g_huart1_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;       /* 无硬件流控 */
    g_huart1_handle.Init.OverSampling = UART_OVERSAMPLING_16;   /* 过采样 */
    HAL_UART_Init(&g_huart1_handle);    /* 初始化并使能串口 */
    HAL_UART_Receive_IT(&g_huart1_handle, g_rx_buf, RX_BUF_SIZE);   /* 开启接收中断 */
}

/**
 * @note 在HAL库中，该函数被弱定义，并且被HAL_UART_Init函数调用，用户通常可以用来实现一些串口初始化底层硬件相关操作
 *      这里用来进行UART1的GPIO、时钟和中断相关配置，以更好实现代码分层
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart){
    /* 首先判断串口是否是UART1 */
    if(huart->Instance == USART1){
        RCC_UART1_CLK_ENABLE();  /* 使能串口和PA9、PA10引脚时钟 */
        /* GPIO相关配置 */
        /* PA9(TX)上拉复用推挽输出， PA10(RX)上拉浮空输入 */
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* 中断相关配置 */
        HAL_NVIC_EnableIRQ(USART1_IRQn);  /* 使能串口1中断通道 */
        HAL_NVIC_SetPriority(USART1_IRQn, 2, 0);  /* 设置优先级 */
    }
}

/* 中断服务函数 */
void USART1_IRQHandler(void){
    /* 调用UART公中断处理函数，完成挂起位等相关位的复位，为下次中断触发提供条件，同时调用中断回调函数 */
    HAL_UART_IRQHandler(&g_huart1_handle);   
}

/**
 * @note 接收完成中断回调函数
 *      通常在HAL_UART_IRQHandler函数中触发HAL_UART_Receive_IT函数，当接收完成时，会触发该回调函数
 *      该函数被弱定义，用户可以重写该函数以实现串口接收完成后的操作
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    /* 判断串口号 */
    if(huart->Instance == USART1){
        /* 接收完成，重新开启接收中断 */
        /* 判断是否是换行(\r\n)，如果是，表示收到了一个完整指令 */
        if(g_rx_buf[0] == '\n'){
            /* 处理接收到的数据 */
            /* 在中断中最好不做数据处理，这里使用定义的标志位，把指令接收标志位置1即可 */
            g_rx_flag = true;
        }else{
            /* 如果不是换行，则将接收到的数据存入接收暂存区 */
            g_UART_rx_buf[g_rx_len++] = g_rx_buf[0];
        }
        /* 重新开启接收中断 */
        HAL_UART_Receive_IT(&g_huart1_handle, g_rx_buf, RX_BUF_SIZE);
    }
}

/**
 * @note 在接收到一条完整指令后，解析指令执行对应操作
 *      这里只是简单把指令通过串口原样返回到上位机
 */
void UART1_Process(void){
    if(g_rx_flag){
        /* 处理接收到的数据 */
        /* 把接收到的数据移交到发送缓存区 */
        memcpy(g_tx_buf, g_UART_rx_buf, g_rx_len);
        /* 清空接收暂存区内容 */
        memset(g_UART_rx_buf, 0, RX_LEN);
        HAL_UART_Transmit(&g_huart1_handle, g_tx_buf, g_rx_len, HAL_MAX_DELAY);
        /* 归零暂存区长度 */
        g_rx_len = 0;
        /* 清空接收暂存区接收完成标志 */
        g_rx_flag = false;
    }
}
