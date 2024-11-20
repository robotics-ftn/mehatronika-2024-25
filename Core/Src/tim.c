/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "tim.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* TIM10 init function */
void MX_TIM10_Init(void)
{

//  /* USER CODE BEGIN TIM10_Init 0 */
//
//  /* USER CODE END TIM10_Init 0 */
//
//  LL_TIM_InitTypeDef TIM_InitStruct = {0};
//  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
//
//  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
//  /* Peripheral clock enable */
//  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM10);
//
//  /* USER CODE BEGIN TIM10_Init 1 */
//
//  /* USER CODE END TIM10_Init 1 */
//  TIM_InitStruct.Prescaler = 83;
//  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
//  TIM_InitStruct.Autoreload = 19999;
//  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
//  LL_TIM_Init(TIM10, &TIM_InitStruct);
//  LL_TIM_EnableARRPreload(TIM10);
//  LL_TIM_OC_EnablePreload(TIM10, LL_TIM_CHANNEL_CH1);
//  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
//  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
//  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
//  TIM_OC_InitStruct.CompareValue = 0;
//  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
//  LL_TIM_OC_Init(TIM10, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);
//  LL_TIM_OC_DisableFast(TIM10, LL_TIM_CHANNEL_CH1);
//  /* USER CODE BEGIN TIM10_Init 2 */
//
//  /* USER CODE END TIM10_Init 2 */
//  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
//    /**TIM10 GPIO Configuration
//    PB8     ------> TIM10_CH1
//    */
//  GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
//  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
//  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
//  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
//  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
//  GPIO_InitStruct.Alternate = LL_GPIO_AF_3;
//  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	/*
		 * TIM10 CH1
		 * f_pwm = 50Hz
		 * PB8 AF3
		 */

		// Enable GPIO clock
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
		// Enable TIM clock
		RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;

		GPIOB->MODER &= ~(0b11 << 8 * 2);
		GPIOB->MODER |=  (0b10 << 8 * 2);

		GPIOB->AFR[1] &= ~(0b1111 << 0 * 4);
		GPIOB->AFR[1] |=  (3      << 0 * 4);

		// Init timer

		//50Hz
		TIM10->PSC = 84 -1; 	// --> 1 MHz
		TIM10->ARR = 20000 - 1;	// 50 Hz

		// rezim rada PWM Mode 1, logicka jedinica
		// sve dok je CNT < CCR
		TIM10->CCMR1 &= ~(0b111 << 4);
		TIM10->CCMR1 |=  (0b110 << 4);

		// enable preload
		TIM10->CCMR1 |= (0b1 << 3);

		TIM10->CR1 |= (0b1 << 7);

		//enable CH1 output
		TIM10->CCER |= (0b1 << 0);


		TIM10->EGR |= (0b1 << 0);
		// Enable counter
		TIM10->CR1 |= (0b1 << 0);
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
