/**
* @file    PS2.h
  * @brief   PlayStation 2 (PS2) controller driver for STM32 HAL
  * @author  Shuai
  * @date    Created: 2025-08-12
  * @version V1.0
  * @attention
  *  - Requires STM32 HAL library
  *  - Pin configuration in CubeMX: PB12(DATA), PB13(CMD), PB14(CS/ATT), PB15(CLK)
  */

#ifndef PS2_H
#define PS2_H
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Type Definitions ----------------------------------------------------------*/
/**
  * @brief PS2 Controller Button Mapping
  */
typedef struct
{
    uint8_t mode; /* 手柄的工作模式 */

    uint8_t btn1; /* B0:SLCT B1:JR  B0:JL B3:STRT B4:UP B5:R B6:DOWN  B7:L   */

    uint8_t btn2; /* B0:L2   B1:R2  B2:L1 B3:R1   B4:Y  B5:B B6:A     B7:X */

    uint8_t RJoy_LR; /* 右边摇杆  0x00 = 左    0xff = 右   */

    uint8_t RJoy_UD; /* 右边摇杆  0x00 = 上    0xff = 下   */

    uint8_t LJoy_LR; /* 左边摇杆  0x00 = 左    0xff = 右   */

    uint8_t LJoy_UD; /* 左边摇杆  0x00 = 上    0xff = 下   */
} JOYSTICK_TypeDef;

/* Exported Functions --------------------------------------------------------*/
/**
  * @brief  Initialize the PS2 controller interface
  * @note   Sets default pin states, must be called before communication
  */
void PS2_Init(void);
/**
  * @brief  Read current controller state
  * @param  ctrl Pointer to controller state structure
  * @retval uint8_t Connection status (1=connected, 0=disconnected)
  */
void PS2_ScanKey(JOYSTICK_TypeDef* JoystickStruct);

#endif //PS2_H
