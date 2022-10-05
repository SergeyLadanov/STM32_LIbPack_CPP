#include "SerialHardware.hpp"

SerialHardware SerialObj;


.................

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	SerialObj.UpdateBufferIndex(Size - 1);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	SerialObj.TxCompleteHandle();
}


static void Serial_Init(void)
{
	constexpr uint16_t UART_BUFFER_SIZE = 128;
	static uint8_t uart_buf[UART_BUFFER_SIZE];
	SerialObj = SerialHardware(uart_buf, sizeof(uart_buf), &huart6);
	SerialObj.SetTimeOutVal(50);
	SerialObj.InitOsAttributes();
	// DMA is using circular receiving
	HAL_UARTEx_ReceiveToIdle_DMA(&huart6, uart_buf, sizeof(uart_buf));

}


/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	Serial_Init();
	char str[256];
  /* Infinite loop */
  for(;;)
  {
	  char *data_ptr = nullptr;

	  if (SerialObj.WaitForData(osWaitForever))
	  {
		  data_ptr = SerialObj.GetBytes();
		  sprintf(str, "Repply: %s\r\n", data_ptr);
		  SerialObj.SendData((uint8_t *) str, strlen(str));
	  }


  }
  /* USER CODE END 5 */
}

.........................................
/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  if (htim->Instance == TIM2)
  {
	  SerialObj.TickHandle();
  }
  /* USER CODE END Callback 1 */
}