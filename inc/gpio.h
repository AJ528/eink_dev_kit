
#ifndef __GPIO_H
#define __GPIO_H

#include <stdint.h>
#include <stdbool.h>

void GPIO_init(void);
void EPD_reset(void);
bool EPD_is_busy(void);
void EPD_WriteCMD(uint8_t command);
void EPD_WriteData(uint8_t data);

#endif /* __GPIO_H */