/**
* @file    PS2.c
  * @brief   PlayStation 2 (PS2) controller driver implementation
  * @author  Shuai
  * @date    Created: 2025-08-12
  * @version V1.0
  */
/* Includes ------------------------------------------------------------------*/
#include "PS2.h"

/* Private defines -----------------------------------------------------------*/
// Pin mapping macros

#define DI()      HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)

// 命令引脚设置
#define CMD_H()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET)
#define CMD_L()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET)

// 片选引脚设置
#define CS_H()    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET)
#define CS_L()    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET)

// 时钟引脚设置
#define CLK_H()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET)
#define CLK_L()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET)

/* Private variables ---------------------------------------------------------*/

const uint8_t PS2_cmnd[9] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //请求获取数据命令
static uint8_t PS2_data[9] = {0}; //接收的数据


/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initialize PS2 controller interface
  */
void PS2_Init(void)
{
    CS_H();
    CLK_H();
}

/**
  * @brief  Microsecond delay for timing critical operations
  * @param  us Delay in microseconds
  * @note   Optimized for Cortex-M processors with 0-wait-state memory
  */
void Delayus(uint16_t us)
{
    uint32_t ticks = us * (SystemCoreClock / 8000000);
    while (ticks--)
        __NOP();
}


/**
  * @简  述  PS2数据读写函数
  * @参  数  cmd:要写入的命令
  * @返回值  读出数据
  */
static uint8_t PS2_ReadWriteData(uint8_t cmd)
{
    volatile uint8_t res = 0;
    volatile uint8_t ref;

    //写入命令，并读取一个1字节数据
    for (ref = 0x01; ref > 0x00; ref <<= 1)
    {
        ////输出一位数据
        if (ref & cmd)
            CMD_H();
        else
            CMD_L();

        CLK_L();
        Delayus(16);

        //读取一位数据
        if (DI())
            res |= ref;
        CLK_H();
        Delayus(16);
    }

    //返回读出数据
    return res;
}

/**
  * @简  述  PS2获取按键及摇杆数值。
  * @参  数  *JoystickStruct 手柄键值结构体
  * @返回值  无
  */
void PS2_ScanKey(JOYSTICK_TypeDef* JoystickStruct)
{
    uint8_t i;

    //使能手柄
    CS_L();

    //读取PS2数据
    for (i = 0; i < 9; i++)
    {
        PS2_data[i] = PS2_ReadWriteData(PS2_cmnd[i]);
    }

    //关闭使能
    CS_H();

    //数值传递
    JoystickStruct->mode = PS2_data[1];
    JoystickStruct->btn1 = ~PS2_data[3];
    JoystickStruct->btn2 = ~PS2_data[4];
    JoystickStruct->RJoy_LR = PS2_data[5];
    JoystickStruct->RJoy_UD = PS2_data[6];
    JoystickStruct->LJoy_LR = PS2_data[7];
    JoystickStruct->LJoy_UD = PS2_data[8];
}
