/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "gpio.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "pikaScript.h"
#include "pikaVM.h"
#include "stdbool.h"
#include "pika_config.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* supply the main object */
PikaObj* pikaMain;
char Shell_Buff[RX_BUFF_LENGTH] = {0};
uint8_t Shell_Ready = 0;
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */

#if use_mem_pool
extern Pool pikaPool;
#endif

int main(void) {
    /* support bootLoader */
    __disable_irq();
    SCB->VTOR = FLASH_BASE | 0x2000;
    __enable_irq();
    
    /* system init */
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    HARDWARE_PRINTF_Init();
    printf("[info]: stm32 hardware init ok\r\n");
    
    /* init mem pool */
    #if use_mem_pool
    pikaPool = pool_init(0x1800, 4);
    printf("[info]: pika memory poool init ok \r\n");
    #endif

    /* boot pikaScript */
    char* code = (char*)FLASH_SCRIPT_START_ADDR;
    if (code[0] != 0xFF) {
        /* boot from flash */
        pikaMain = newRootObj("pikaMain", New_PikaMain);
        if (code[0] == 'i') {
            printf("[info]: boot from Script.\r\n");
            Arg* codeBuff = arg_setStr(NULL, "", code);
            obj_run(pikaMain, arg_getStr(codeBuff));
            arg_deinit(codeBuff);
            goto main_loop;
        }
        if (code[0] == 'B') {
            printf("==============[Pika ASM]==============\r\n");
            for (int i = 0; i < strGetSize(code); i++) {
                if ('\n' == code[i]) {
                    fputc('\r', (FILE*)!NULL);
                }
                fputc(code[i], (FILE*)!NULL);
            }
            printf("==============[Pika ASM]==============\r\n");
            printf("[info]: asm size: %d\r\n", strGetSize(code));
            printf("[info]: boot from Pika Asm.\r\n");
            pikaVM_runAsm(pikaMain, code);
            goto main_loop;
        }
    } else {
        /* boot from firmware */
        pikaMain = pikaScriptInit();
        goto main_loop;
    }

    pikaMain = pikaScriptInit();
    goto main_loop;

main_loop:
    while (1) {
        if(Shell_Ready){
            Shell_Ready = 0;
            obj_run(pikaMain, Shell_Buff);
        }
    }
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Configure the main internal regulator output voltage
     */
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
    RCC_OscInitStruct.PLL.PLLN = 16;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV3;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the peripherals clocks
     */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state
     */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line
       number, ex: printf("Wrong parameters value: file %s on line %d\r\n",
       file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

