/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "sdio.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
LED STAT = STAT_INIT();
LED SAVE = SAVE_INIT();
LED PRGM = PRGM_INIT();
LED RDY = RDY_INIT();

MS5607 BARO1 = BARO1_INIT();
MS5607 BARO2 = BARO2_INIT();

ICM20601 IMU1 = IMU1_INIT();
ICM20601 IMU2 = IMU2_INIT();

SHT31 TEMP = SHT_INIT();
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint32_t tick;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI2_Init();
  MX_ADC1_Init();
  MX_I2C2_Init();
  MX_SDIO_SD_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  turn_on(&STAT);
  HAL_Delay(300);
  turn_on(&SAVE);
  HAL_Delay(300);
  turn_on(&PRGM);
  HAL_Delay(300);

  turn_off(&STAT);
  turn_off(&SAVE);
  turn_off(&PRGM);
  HAL_Delay(1000);


  ms5607_init(&BARO1);
  ms5607_init(&BARO2);

  if (icm20601_init(&IMU1)) printf("setup IMU1 successful \n");
  if (icm20601_init(&IMU2)) printf("setup IMU2 successful \n");

  sht31_init(&TEMP);

  uint8_t raw_data1[3];
  uint8_t raw_data2[3];

  float p1 = 0;
  float p2 = 0;
  float t_p1 = 0;
  float t_p2 = 0;

  int16_t accel1_raw_buf[6];
  int16_t accel2_raw_buf[6];

  float accel1_val[6];
  float accel2_val[6];

  uint16_t t_buf[2];
  float t_val[2];

  HAL_Delay(1000);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	tick = HAL_GetTick();

	if (LED_is_busy(&STAT)) toggle(&STAT);

	if (LED_is_busy(&SAVE)) toggle(&SAVE);

	if (LED_is_busy(&PRGM)) toggle(&PRGM);

	if (LED_is_busy(&RDY)) toggle(&RDY);


	if (sht31_is_busy(&TEMP))
	{
		sht31_read(&TEMP, t_val, t_buf);
	}


	//icm20601_read_data_raw(&IMU1, accel1_raw_buf);
	//icm20601_convert_data(&IMU1, accel1_val, accel1_raw_buf);
	icm20601_read_data(&IMU1, accel1_val);

	//icm20601_read_data_raw(&IMU2, accel2_raw_buf);
	//icm20601_convert_data(&IMU2, accel2_val, accel2_raw_buf);
	icm20601_read_data(&IMU2, accel2_val);



	if (ms5607_is_busy(&BARO1, 3, MS_TEMPERATURE_REQ))
	{
		ms5607_prep_temp(&BARO1);
	}
	if (ms5607_is_busy(&BARO2, 3, MS_TEMPERATURE_REQ))
	{
		ms5607_prep_temp(&BARO2);
	}
	//delay 3ms

	//HAL_Delay(5);

	if (ms5607_is_busy(&BARO1, 3, MS_PRESSURE_REQ))
	{
		ms5607_prep_pressure(&BARO1, raw_data1);
	}
	if (ms5607_is_busy(&BARO2, 3, MS_PRESSURE_REQ))
	{
		ms5607_prep_pressure(&BARO2, raw_data2);
	}
	//delay 3ms

	//HAL_Delay(5);

	if (ms5607_is_busy(&BARO1, 3, MS_DATA_READOUT)) {
		ms5607_read_pressure(&BARO1, raw_data1);
		ms5607_convert(&BARO1, &p1, &t_p1);
	}
	if (ms5607_is_busy(&BARO2, 3, MS_DATA_READOUT)) {
		ms5607_read_pressure(&BARO2, raw_data2);
		ms5607_convert(&BARO2, &p2, &t_p2);
	}




	printf("p1 = %4.2f bar and t1 = %4.2f C \n",p1,t_p1);
	printf("p2 = %4.2f bar and t2 = %4.2f C \n",p2,t_p2);
	printf("T = %4.2f C and H = %4.2f perc \n",t_val[0],t_val[1]);
	printf("IMU1 T: %4.2f C \n", accel1_val[0]);
	printf("IMU1 ax: %4.2f m/s2 \n", accel1_val[1]);
	printf("IMU1 ay: %4.2f m/s2 \n", accel1_val[2]);
	printf("IMU1 az: %4.2f m/s2 \n", accel1_val[3]);
	printf("IMU2 T: %4.2f C \n", accel2_val[0]);
	printf("IMU2 ax: %4.2f m/s2 \n", accel2_val[1]);
	printf("IMU2 ay: %4.2f m/s2 \n", accel2_val[2]);
	printf("IMU2 az: %4.2f m/s2 \n", accel2_val[3]);

	HAL_Delay(1000);

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
