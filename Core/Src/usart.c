/**
 ******************************************************************************
 * File Name          : USART.c
 * Description        : This file provides code for the configuration
 *                      of the USART instances.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* USART6 init function */

void
MX_USART6_UART_Init (void)
{
  LL_USART_InitTypeDef USART_InitStruct =
    { 0 };

  LL_GPIO_InitTypeDef GPIO_InitStruct =
    { 0 };

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock (LL_APB2_GRP1_PERIPH_USART6);

  LL_AHB1_GRP1_EnableClock (LL_AHB1_GRP1_PERIPH_GPIOA);
  /**USART6 GPIO Configuration  
   PA11   ------> USART6_TX
   PA12   ------> USART6_RX 
   */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
  LL_GPIO_Init (GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_12;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
  LL_GPIO_Init (GPIOA, &GPIO_InitStruct);

  /* USART6 interrupt Init */
  NVIC_SetPriority (USART6_IRQn,
		    NVIC_EncodePriority (NVIC_GetPriorityGrouping (), 0, 0));
  NVIC_EnableIRQ (USART6_IRQn);

  USART_InitStruct.BaudRate = 9600;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init (USART6, &USART_InitStruct);
  LL_USART_Enable (USART6);
  LL_USART_ConfigHalfDuplexMode (USART6);

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
