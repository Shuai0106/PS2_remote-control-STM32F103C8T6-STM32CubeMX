# PS2_remote-control-STM32F103C8T6-STM32CubeMX
Professional implementation of a PS2 controller driver for STM32 microcontrollers using the HAL library.
## Features
- ✅ Supports all PS2 controller buttons and analog sticks
- 🔄 Automatic detection of controller mode (analog/digital)
- ⚡ Optimized communication protocol with precise timing
- 📡 Easy-to-use API for reading controller state
- 🔧 Configurable GPIO pins
## Hardware Setup
| PS2 Pin | STM32 Connection |
|---------|------------------|
| DATA    | PB12            |
| CMD     | PB13            |
| CS      | PB14            |
| CLK     | PB15            |
| VCC     | 3.3V / 5V (需分压) |
| GND     | GND             |
## Usage
```c
通过串口接收遥控器的值
#include "PS2.h"
PS2_Controller_t ps2;
uint32_t last_read = HAL_GetTick();
int main(void)
{
  // 初始化
  HAL_Init();
  PS2_Init();
   while (1)
  {
    /* USER CODE END WHILE */
    JOYSTICK_TypeDef joy;
    PS2_ScanKey(&joy);

    char buf[128];
    snprintf(buf, sizeof(buf),
        "M:%02X B1:%02X B2:%02X RX:%3d RY:%3d LX:%3d LY:%3d\n",
        joy.mode, joy.btn1, joy.btn2,
        joy.RJoy_LR, joy.RJoy_UD, joy.LJoy_LR, joy.LJoy_UD);

    HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), 100);
    HAL_UART_Transmit(&huart1, (uint8_t*)"\r\n", 2, 100);
    HAL_Delay(100); // 适当延时
  }
}
