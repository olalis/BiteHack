#include "lps25hb.h"
#include "i2c.h"

#define LPS25HB_ADDR			(uint16_t)0xBA

#define LPS25HB_WHO_AM_I 		0x0F
#define LPS25HB_CTRL_REG1 		0x20
#define LPS25HB_CTRL_REG2 		0x21
#define LPS25HB_CTRL_REG3 		0x22
#define LPS25HB_CTRL_REG4 		0x23
#define LPS25HB_PRESS_OUT_XL 	0x28
#define LPS25HB_PRESS_OUT_L 	0x29
#define LPS25HB_PRESS_OUT_H 	0x2A
#define LPS25HB_TEMP_OUT_L 		0x2B
#define LPS25HB_TEMP_OUT_H 		0x2C

#define LPS25HB_CTRL_REG1_PD 	0x80
#define LPS25HB_CTRL_REG1_ODR2 	0x40
#define LPS25HB_CTRL_REG1_ODR1 	0x20
#define LPS25HB_CTRL_REG1_ODR0 	0x10

#define LPS25HB_RPDS_L 			0x39
#define LPS25HB_RPDS_H 			0x3A


static uint8_t Lps25hb_ReadReg(uint8_t reg);
void Lps25hb_WriteReg(uint8_t reg, uint8_t value);


void Lps25hb_Init(void)
{
	Lps25hb_WriteReg(LPS25HB_CTRL_REG1, LPS25HB_CTRL_REG1_PD | LPS25HB_CTRL_REG1_ODR1);
	HAL_Delay(100);
}

void Lps25hb_Read(float* pressure, float* temperature)
{
	int16_t temp;
	int32_t pres = 0;

	HAL_I2C_Mem_Read(&hi2c4, LPS25HB_ADDR, LPS25HB_TEMP_OUT_L | 0x80, 1, (uint8_t*)&temp, sizeof(temp), HAL_MAX_DELAY);
	HAL_I2C_Mem_Read(&hi2c4, LPS25HB_ADDR, LPS25HB_PRESS_OUT_XL | 0x80, 1, (uint8_t*)&pres, 3, HAL_MAX_DELAY);

	*temperature =  42.5f + temp / 480.0f;
	*pressure = pres / 4096.0f;
}

void Lps25hb_SetCalib(uint16_t value)
{
	Lps25hb_WriteReg(LPS25HB_RPDS_L, value);
	Lps25hb_WriteReg(LPS25HB_RPDS_H, value >> 8);
}


static uint8_t Lps25hb_ReadReg(uint8_t reg)
{
	uint8_t value = 0;
	HAL_I2C_Mem_Read(&hi2c4, LPS25HB_ADDR, reg, 1, &value, sizeof(value), HAL_MAX_DELAY);

	return value;
}


void Lps25hb_WriteReg(uint8_t reg, uint8_t value)
{
	HAL_I2C_Mem_Write(&hi2c4, LPS25HB_ADDR, reg, 1, &value, sizeof(value), HAL_MAX_DELAY);
}


