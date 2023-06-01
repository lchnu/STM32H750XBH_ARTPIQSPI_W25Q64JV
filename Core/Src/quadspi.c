/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    quadspi.c
  * @brief   This file provides code for the configuration
  *          of the QUADSPI instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "quadspi.h"
#include <string.h>
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */


//QSPI_HandleTypeDef hqspi;

///* QUADSPI init function */
//void MX_QUADSPI_Init(void)
//{

//  /* USER CODE BEGIN QUADSPI_Init 0 */

//  /* USER CODE END QUADSPI_Init 0 */

//  /* USER CODE BEGIN QUADSPI_Init 1 */

//  /* USER CODE END QUADSPI_Init 1 */
//  hqspi.Instance = QUADSPI;
//  hqspi.Init.ClockPrescaler = 1;
//  hqspi.Init.FifoThreshold = 32;
//  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
//  hqspi.Init.FlashSize = 22;
//  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_5_CYCLE;
//  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
//  hqspi.Init.FlashID = QSPI_FLASH_ID_1;
//  hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
//  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN QUADSPI_Init 2 */

//  /* USER CODE END QUADSPI_Init 2 */

//}

//void HAL_QSPI_MspInit(QSPI_HandleTypeDef* qspiHandle)
//{

//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
//  if(qspiHandle->Instance==QUADSPI)
//  {
//  /* USER CODE BEGIN QUADSPI_MspInit 0 */

//  /* USER CODE END QUADSPI_MspInit 0 */

//  /** Initializes the peripherals clock
//  */
//    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_QSPI;
//    PeriphClkInitStruct.QspiClockSelection = RCC_QSPICLKSOURCE_D1HCLK;
//    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
//    {
//      Error_Handler();
//    }

//    /* QUADSPI clock enable */
//    __HAL_RCC_QSPI_CLK_ENABLE();

//    __HAL_RCC_GPIOG_CLK_ENABLE();
//    __HAL_RCC_GPIOF_CLK_ENABLE();
//    /**QUADSPI GPIO Configuration
//    PG6     ------> QUADSPI_BK1_NCS
//    PF6     ------> QUADSPI_BK1_IO3
//    PF7     ------> QUADSPI_BK1_IO2
//    PF8     ------> QUADSPI_BK1_IO0
//    PF10     ------> QUADSPI_CLK
//    PF9     ------> QUADSPI_BK1_IO1
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_6;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
//    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

//    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_10;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
//    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

//    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
//    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

//  /* USER CODE BEGIN QUADSPI_MspInit 1 */

//  /* USER CODE END QUADSPI_MspInit 1 */
//  }
//}

//void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef* qspiHandle)
//{

//  if(qspiHandle->Instance==QUADSPI)
//  {
//  /* USER CODE BEGIN QUADSPI_MspDeInit 0 */

//  /* USER CODE END QUADSPI_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_QSPI_CLK_DISABLE();

//    /**QUADSPI GPIO Configuration
//    PG6     ------> QUADSPI_BK1_NCS
//    PF6     ------> QUADSPI_BK1_IO3
//    PF7     ------> QUADSPI_BK1_IO2
//    PF8     ------> QUADSPI_BK1_IO0
//    PF10     ------> QUADSPI_CLK
//    PF9     ------> QUADSPI_BK1_IO1
//    */
//    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_6);

//    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_10
//                          |GPIO_PIN_9);

//  /* USER CODE BEGIN QUADSPI_MspDeInit 1 */

//  /* USER CODE END QUADSPI_MspDeInit 1 */
//  }
//}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
