#include "comm.h"
#include "stdint.h"
#include "usart.h"

HAL_StatusTypeDef Comm_SendStr(char* msg)
{
	return HAL_UART_Transmit(&huart3, (uint8_t*)msg, strlen(msg), 10000);
}


HAL_StatusTypeDef Comm_SendBytes(uint8_t* msg, uint16_t len)
{
	return HAL_UART_Transmit(&huart3, msg, len, 10000);
}

