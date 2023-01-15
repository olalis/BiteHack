#ifndef SRC_AM2320_H_
#define SRC_AM2320_H_

#include "stdint.h"
#include "i2c.h"

void Am2320_Read(I2C_HandleTypeDef* i2c_handle, float* humidity, float* temperature);

#endif /* SRC_AM2320_H_ */
