#ifndef __DISPLAY_EPD_H
#define __DISPLAY_EPD_H

#include <stdint.h>

void EPD_init(void);
void EPD_update(void);
void EPD_deepsleep(void);

void EPD_whitescreen_white(void);
void EPD_whitescreen_all(const uint8_t *data);


#endif /* __DISPLAY_EPD_H */
