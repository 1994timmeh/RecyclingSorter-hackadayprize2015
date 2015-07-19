#ifndef __5110__
#define __5110__
// Includes
#include <stdint.h>
#include "board.h"
#include "stm32f4xx_hal_conf.h"
#include "debug_printf.h"

// Definitions
#define LCD_DC_PORT BRD_D9_GPIO_PORT
#define LCD_DC_PIN BRD_D9_PIN

#define LCD_RST_PORT BRD_D8_GPIO_PORT
#define LCD_RST_PIN BRD_D8_PIN

#define command_mode(); { HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, (GPIO_PinState)0); }
#define data_mode(); { HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, (GPIO_PinState)1); }

#define CE_LOW(); { HAL_GPIO_WritePin(BRD_SPI_CS_GPIO_PORT, BRD_SPI_CS_PIN, (GPIO_PinState)0); }
#define CE_HIGH(); { HAL_GPIO_WritePin(BRD_SPI_CS_GPIO_PORT, BRD_SPI_CS_PIN, (GPIO_PinState)1); }

// Functions
uint8_t LCD_SendByte(uint8_t sendbyte);
void LCD_Test(uint8_t data);
void LCD_RST();
void LCD_Init();
void LCD_Letter(uint8_t x);
void LCD_String(uint8_t* x);
void LCD_WriteLine(uint8_t* x, uint8_t center);
void LCD_ProgressBar();
void Delay(__IO unsigned long nCount);


// Globals
SPI_HandleTypeDef SpiHandle;

#endif //__5110__
