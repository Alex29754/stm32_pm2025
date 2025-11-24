#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

void SSD1306_Init(void);
void SSD1306_Clear(void);
void SSD1306_Chessboard(void);
void SSD1306_Command(uint8_t cmd);
void SSD1306_Data(uint8_t data);

#endif