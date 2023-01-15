#include "am2320.h"

#define AM2320_I2C_ADDR		(uint16_t)((0x5C << 1))


void Am2320_Read(I2C_HandleTypeDef* i2c_handle, float* humidity, float* temperature)
{
	uint8_t registers[3] = { 0x03, 0x00, 0x04 };
	uint8_t rx_data[8];

	static uint32_t err_code;


	HAL_I2C_Master_Transmit(i2c_handle, AM2320_I2C_ADDR, 0x00, 0, HAL_MAX_DELAY);

	if (HAL_I2C_Master_Transmit(i2c_handle, AM2320_I2C_ADDR, registers, 3, HAL_MAX_DELAY) != HAL_OK)
	{
		err_code++;
	}

	if (HAL_I2C_Master_Receive(i2c_handle, AM2320_I2C_ADDR, rx_data, 8, HAL_MAX_DELAY) != HAL_OK)
	{
		err_code++;
	}

	if (rx_data[1] != 0x04 && rx_data[0] != 0x03)
	{
		err_code++;
	}

	uint16_t temp_temperature = (rx_data[5] | rx_data[4] << 8);

	if (temp_temperature & 0x8000) {
		temp_temperature = -(int16_t) (temp_temperature & 0x7fff);
	} else {
		temp_temperature = (int16_t) temp_temperature;
	}
	*temperature = (float)temp_temperature / 10.0f;
	*humidity = (float)(rx_data[3] | rx_data[2] << 8) / 10.0f;
}


