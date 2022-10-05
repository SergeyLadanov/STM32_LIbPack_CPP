#include "SerialHardware.hpp"

HardwareSerial SerialObj;


.................

// Инициализация UART
static void Serial_Init(void)
{
	static uint8_t rx_byte;
	static uint8_t uart_buf[UART_BUFFER_SIZE];
	SerialObj = HardwareSerial(uart_buf, sizeof(uart_buf), &huart1);
	SerialObj.SetTimeOutVal(3);
	HAL_UART_Receive_IT(&huart1, &rx_byte, sizeof(rx_byte));
}


/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */
  SerialObj.TickHandle();

  /* USER CODE END SysTick_IRQn 1 */
}





// Обработчик прерываниий UART
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
	{
		SerialObj.ByteHandle(*(huart->pRxBuffPtr - 1));
	}

	HAL_UART_Receive_IT(huart, (huart->pRxBuffPtr - 1), huart->RxXferSize);
}

// Обработчик ошибок UART
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	HAL_UART_Receive_IT(huart, huart->pRxBuffPtr, huart->RxXferSize);
}