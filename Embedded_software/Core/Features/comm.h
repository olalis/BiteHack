#ifndef FEATURES_COMM_H_
#define FEATURES_COMM_H_

#include "string.h"
#include "stm32f7xx_hal.h"


HAL_StatusTypeDef Comm_SendStr(char* msg);
HAL_StatusTypeDef Comm_SendBytes(uint8_t* msg, uint16_t len);

#endif /* FEATURES_COMM_H_ */
