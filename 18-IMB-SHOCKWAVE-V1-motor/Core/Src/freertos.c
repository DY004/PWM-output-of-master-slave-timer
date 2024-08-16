/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "PWR_ON_OFF.h"
#include "tim.h"
#include "motor_control.h"

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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId PWR_TaskHandle;
osThreadId LED_TaskHandle;
osThreadId liquid_TaskHandle;
osThreadId water_pump_TaskHandle;
osThreadId B3950_TaskHandle;
osThreadId PID_TaskHandle;
osThreadId PID_printf_TaskHandle;
osThreadId VOFA_TaskHandle;
osThreadId voice_TaskHandle;
osThreadId servomotor_TaskHandle;
osSemaphoreId Binary_pump_flagHandle;
osSemaphoreId Binary_pump2_flagHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Start_PWR_Task(void const * argument);
void Start_LED_Task(void const * argument);
void Start_liquid_Task(void const * argument);
void Start_water_pump_Task(void const * argument);
void Start_B3950_Task(void const * argument);
void Start_PID_Task(void const * argument);
void Start_PID_printf_Task(void const * argument);
void Start_VOFA_Task(void const * argument);
void Start_voice_Task(void const * argument);
void Start_servomotor_Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of Binary_pump_flag */
  osSemaphoreDef(Binary_pump_flag);
  Binary_pump_flagHandle = osSemaphoreCreate(osSemaphore(Binary_pump_flag), 1);

  /* definition and creation of Binary_pump2_flag */
  osSemaphoreDef(Binary_pump2_flag);
  Binary_pump2_flagHandle = osSemaphoreCreate(osSemaphore(Binary_pump2_flag), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of PWR_Task */
  osThreadDef(PWR_Task, Start_PWR_Task, osPriorityIdle, 0, 128);
  PWR_TaskHandle = osThreadCreate(osThread(PWR_Task), NULL);

  /* definition and creation of LED_Task */
  osThreadDef(LED_Task, Start_LED_Task, osPriorityIdle, 0, 128);
  LED_TaskHandle = osThreadCreate(osThread(LED_Task), NULL);

  /* definition and creation of liquid_Task */
  osThreadDef(liquid_Task, Start_liquid_Task, osPriorityIdle, 0, 128);
  liquid_TaskHandle = osThreadCreate(osThread(liquid_Task), NULL);

  /* definition and creation of water_pump_Task */
  osThreadDef(water_pump_Task, Start_water_pump_Task, osPriorityIdle, 0, 128);
  water_pump_TaskHandle = osThreadCreate(osThread(water_pump_Task), NULL);

  /* definition and creation of B3950_Task */
  osThreadDef(B3950_Task, Start_B3950_Task, osPriorityIdle, 0, 128);
  B3950_TaskHandle = osThreadCreate(osThread(B3950_Task), NULL);

  /* definition and creation of PID_Task */
  osThreadDef(PID_Task, Start_PID_Task, osPriorityIdle, 0, 128);
  PID_TaskHandle = osThreadCreate(osThread(PID_Task), NULL);

  /* definition and creation of PID_printf_Task */
  osThreadDef(PID_printf_Task, Start_PID_printf_Task, osPriorityIdle, 0, 128);
  PID_printf_TaskHandle = osThreadCreate(osThread(PID_printf_Task), NULL);

  /* definition and creation of VOFA_Task */
  osThreadDef(VOFA_Task, Start_VOFA_Task, osPriorityIdle, 0, 128);
  VOFA_TaskHandle = osThreadCreate(osThread(VOFA_Task), NULL);

  /* definition and creation of voice_Task */
  osThreadDef(voice_Task, Start_voice_Task, osPriorityIdle, 0, 128);
  voice_TaskHandle = osThreadCreate(osThread(voice_Task), NULL);

  /* definition and creation of servomotor_Task */
  osThreadDef(servomotor_Task, Start_servomotor_Task, osPriorityIdle, 0, 128);
  servomotor_TaskHandle = osThreadCreate(osThread(servomotor_Task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_Start_PWR_Task */
/**
  * @brief  Function implementing the PWR_Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Start_PWR_Task */
void Start_PWR_Task(void const * argument)
{
  /* USER CODE BEGIN Start_PWR_Task */
  /* Infinite loop */
  for(;;)
  {
	  PWR_ON_OFF();
	  osDelay(20);
  }
  /* USER CODE END Start_PWR_Task */
}

/* USER CODE BEGIN Header_Start_LED_Task */
/**
* @brief Function implementing the LED_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_LED_Task */
void Start_LED_Task(void const * argument)
{
  /* USER CODE BEGIN Start_LED_Task */
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);
	  osDelay(100);
  }
  /* USER CODE END Start_LED_Task */
}

/* USER CODE BEGIN Header_Start_liquid_Task */
/**
* @brief Function implementing the liquid_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_liquid_Task */
void Start_liquid_Task(void const * argument)
{
  /* USER CODE BEGIN Start_liquid_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(300);
  }
  /* USER CODE END Start_liquid_Task */
}

/* USER CODE BEGIN Header_Start_water_pump_Task */
/**
* @brief Function implementing the water_pump_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_water_pump_Task */
void Start_water_pump_Task(void const * argument)
{
  /* USER CODE BEGIN Start_water_pump_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(180);
  }
  /* USER CODE END Start_water_pump_Task */
}

/* USER CODE BEGIN Header_Start_B3950_Task */
/**
* @brief Function implementing the B3950_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_B3950_Task */
void Start_B3950_Task(void const * argument)
{
  /* USER CODE BEGIN Start_B3950_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(300);
  }
  /* USER CODE END Start_B3950_Task */
}

/* USER CODE BEGIN Header_Start_PID_Task */
/**
* @brief Function implementing the PID_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_PID_Task */
void Start_PID_Task(void const * argument)
{
  /* USER CODE BEGIN Start_PID_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(500);
  }
  /* USER CODE END Start_PID_Task */
}

/* USER CODE BEGIN Header_Start_PID_printf_Task */
/**
* @brief Function implementing the PID_printf_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_PID_printf_Task */
void Start_PID_printf_Task(void const * argument)
{
  /* USER CODE BEGIN Start_PID_printf_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(150);
  }
  /* USER CODE END Start_PID_printf_Task */
}

/* USER CODE BEGIN Header_Start_VOFA_Task */
/**
* @brief Function implementing the VOFA_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_VOFA_Task */
void Start_VOFA_Task(void const * argument)
{
  /* USER CODE BEGIN Start_VOFA_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(200);
  }
  /* USER CODE END Start_VOFA_Task */
}

/* USER CODE BEGIN Header_Start_voice_Task */
/**
* @brief Function implementing the voice_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_voice_Task */
void Start_voice_Task(void const * argument)
{
  /* USER CODE BEGIN Start_voice_Task */
  /* Infinite loop */
  for(;;)
  {
	  _motor_control();
  }
  /* USER CODE END Start_voice_Task */
}

/* USER CODE BEGIN Header_Start_servomotor_Task */
/**
* @brief Function implementing the servomotor_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_servomotor_Task */
void Start_servomotor_Task(void const * argument)
{
  /* USER CODE BEGIN Start_servomotor_Task */
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_TogglePin(DIR_GPIO_Port,DIR_Pin);
	  osDelay(1000);
  }
  /* USER CODE END Start_servomotor_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
