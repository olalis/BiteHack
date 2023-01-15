#ifndef FEATURES_LPS25HB_H_
#define FEATURES_LPS25HB_H_

#include "stdint.h"

void Lps25hb_Init(void);
void Lps25hb_Read(float* pressure, float* temperature);
void Lps25hb_SetCalib(uint16_t value);

#endif /* FEATURES_LPS25HB_H_ */
