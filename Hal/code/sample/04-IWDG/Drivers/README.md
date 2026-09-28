文件夹内容：

BSP：板级支持包，包括板载外设的驱动支持

CMSIS：ARM提供的CMSIS代码，主要包括各种头文件和启动文件

STM32F1xx_HAL_Driver：ST提供的F1 HAL库驱动代码

SYSTEM：正点原子提供的系统级核心代码，包括sys、delay和usart三个子文件夹，分别提供时钟配置、延时和串口驱动等关键代码，方便工程构建