/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lcd.h"


void SystemClock_Config(void);
static void MX_GPIO_Init(void);

void LCD_TestAll(void)
{
	// Test 1: Easy Text
	LCD_Clear();
	LCD_putString("Hello from LCD!");
	HAL_Delay(1500);
	// Test 2: Formatted output with LCD_print
	LCD_Clear();
	LCD_print("%d.%3d V", 12, 345);
	HAL_Delay(1500);
	// Test 3: Cursorshift of text in secon LCD-line
	LCD_Clear();
	LCD_CursorShift(1, 0);
	LCD_putString("Line 2");
	// Test 4: Create custom character and put it on the rigth side and the
	// first line of LCD. This character has no replaced default special
	// character 'PlusMinus'
	char smiley[8] = {0x00, 0x0A, 0x00, 0x00, 0x11, 0x0E, 0x00, 0x00};
	LCD_createChar(0, smiley);
	LCD_CursorShift(0, 15);
	LCD_putChar(0);
	HAL_Delay(1500);
	// Test 5: Display all standard special characters
	LCD_Clear();
	for (uint8_t i = 0; i < 8; ++i)
	{
		LCD_putChar(i);
		LCD_putString(" ");
	}
	HAL_Delay(2000);
	// Test 6: Put string in first line of the LCD. Excess text will be truncated.
	LCD_Clear();
	LCD_putString("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
	HAL_Delay(1500);
	// Test 7: Set the cursor to home position and display 'Home!' at this
	// position
	LCD_CursorHome();
	LCD_putString("Home!");
	HAL_Delay(1500);
	// Test 8: Cursorshift
	LCD_Clear();
	LCD_CursorShift(0, 5); LCD_putString("A");
	LCD_CursorShift(1, 10); LCD_putString("B");
	HAL_Delay(1500);
	// Test 9: Displays text 'Smiley: ' and special char 'smiley'
	LCD_Clear();
	LCD_CursorShift(1, 0);
	LCD_putString("Smiley: ");
	LCD_putChar(0);
	HAL_Delay(1500);
	// Test 10: New special character 'Nabla' programmmed in slot 3. This has replaced
	// default special character 'Beta'
	LCD_Clear();
	char CharNabla[8] = { 0x1F, 0x0E, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00 };
	LCD_createChar(3, CharNabla);
	LCD_CursorShift(0, 0);
	LCD_putChar(3);
	HAL_Delay(2000);
	// Test 11: Shows all special characters (the defaults and new pragrammed)
	LCD_Clear();
	for (uint8_t i = 0; i < 8; ++i)
	{
	    LCD_putChar(i);
	    LCD_putString(" ");
	}
	HAL_Delay(2000);
	// Test 12: Programmed and displayed the default special characters
	LCD_createDefaultCustomChars();
	LCD_Clear();
	for (uint8_t i = 0; i < 8; ++i)
	{
		LCD_putChar(i);
		LCD_putString(" ");
	}
	// Test 13: Displaying a hexadecimal value (0xAF) on the LCE
	HAL_Delay(2000);
	LCD_Clear();
	uint8_t value = 0xAF;
	LCD_print("Value: 0x%02X", value);
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  LCD_Init();
  LCD_TestAll();

  while (1)
  {

  }

}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}


static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif /* USE_FULL_ASSERT */
