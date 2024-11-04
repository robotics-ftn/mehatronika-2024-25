/**
 ******************************************************************************
 * File Name          : TIM.c
 * Description        : This file provides code for the configuration
 *                      of the TIM instances.
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
#include "tim.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* TIM2 init function */
void
MX_TIM2_Init (void)
{
  LL_TIM_InitTypeDef TIM_InitStruct =
    { 0 };

  LL_GPIO_InitTypeDef GPIO_InitStruct =
    { 0 };

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock (LL_APB1_GRP1_PERIPH_TIM2);

  LL_AHB1_GRP1_EnableClock (LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock (LL_AHB1_GRP1_PERIPH_GPIOB);
  /**TIM2 GPIO Configuration  
   PA15   ------> TIM2_CH1
   PB3   ------> TIM2_CH2 
   */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_15;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init (GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init (GPIOB, &GPIO_InitStruct);

  LL_TIM_SetEncoderMode (TIM2, LL_TIM_ENCODERMODE_X4_TI12);
  LL_TIM_IC_SetActiveInput (TIM2, LL_TIM_CHANNEL_CH1,
			    LL_TIM_ACTIVEINPUT_DIRECTTI);
  LL_TIM_IC_SetPrescaler (TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_ICPSC_DIV1);
  LL_TIM_IC_SetFilter (TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_IC_FILTER_FDIV1);
  LL_TIM_IC_SetPolarity (TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_RISING);
  LL_TIM_IC_SetActiveInput (TIM2, LL_TIM_CHANNEL_CH2,
			    LL_TIM_ACTIVEINPUT_DIRECTTI);
  LL_TIM_IC_SetPrescaler (TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_ICPSC_DIV1);
  LL_TIM_IC_SetFilter (TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_IC_FILTER_FDIV1);
  LL_TIM_IC_SetPolarity (TIM2, LL_TIM_CHANNEL_CH2, LL_TIM_IC_POLARITY_RISING);
  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 65535;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init (TIM2, &TIM_InitStruct);
  LL_TIM_DisableARRPreload (TIM2);
  LL_TIM_SetTriggerOutput (TIM2, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode (TIM2);

}
/* TIM10 init function */
void
MX_TIM10_Init (void)
{
  LL_TIM_InitTypeDef TIM_InitStruct =
    { 0 };

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock (LL_APB2_GRP1_PERIPH_TIM10);

  /* TIM10 interrupt Init */
  NVIC_SetPriority (TIM1_UP_TIM10_IRQn,
		    NVIC_EncodePriority (NVIC_GetPriorityGrouping (), 0, 0));
  NVIC_EnableIRQ (TIM1_UP_TIM10_IRQn);

  TIM_InitStruct.Prescaler = 83;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 999;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init (TIM10, &TIM_InitStruct);
  LL_TIM_DisableARRPreload (TIM10);

}

/* USER CODE BEGIN 1 */
int16_t
tim_brzina_1 ()
{
  int16_t brzina = (int16_t) (TIM2->CNT);
  TIM2->CNT = 0;
  return brzina;
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
