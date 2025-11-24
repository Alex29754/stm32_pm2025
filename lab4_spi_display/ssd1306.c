#include "ssd1306.h"
#include "spi.h"
#include "stm32f10x.h"

void SSD1306_Command(uint8_t cmd) {
    GPIOA->ODR &= ~GPIO_ODR_ODR4; // CS = 0
    GPIOA->ODR &= ~GPIO_ODR_ODR1; // DC = 0 (command)
    SPI1_Write(cmd);
    GPIOA->ODR |= GPIO_ODR_ODR4;  // CS = 1
}

void SSD1306_Data(uint8_t data) {
    GPIOA->ODR &= ~GPIO_ODR_ODR4; // CS = 0
    GPIOA->ODR |= GPIO_ODR_ODR1;  // DC = 1 (data)
    SPI1_Write(data);
    GPIOA->ODR |= GPIO_ODR_ODR4;  // CS = 1
}

void SSD1306_Init(void) {
    // Reset display
    GPIOA->ODR &= ~GPIO_ODR_ODR0; // RES = 0
    for(volatile int i = 0; i < 10000; i++);
    GPIOA->ODR |= GPIO_ODR_ODR0;  // RES = 1
    for(volatile int i = 0; i < 10000; i++);
    
    // Init sequence
    SSD1306_Command(0xAE); // Display OFF
    SSD1306_Command(0x20); SSD1306_Command(0x00); // Horizontal mode
    SSD1306_Command(0xB0); // Page start
    SSD1306_Command(0xC8); // Scan direction
    SSD1306_Command(0x00); // Low column
    SSD1306_Command(0x10); // High column
    SSD1306_Command(0x40); // Start line
    SSD1306_Command(0x81); SSD1306_Command(0xFF); // Contrast
    SSD1306_Command(0xA1); // Segment remap
    SSD1306_Command(0xA6); // Normal display
    SSD1306_Command(0xA8); SSD1306_Command(0x3F); // Multiplex
    SSD1306_Command(0xA4); // Display RAM
    SSD1306_Command(0xD3); SSD1306_Command(0x00); // Offset
    SSD1306_Command(0xD5); SSD1306_Command(0xF0); // Clock div
    SSD1306_Command(0xD9); SSD1306_Command(0x22); // Precharge
    SSD1306_Command(0xDA); SSD1306_Command(0x12); // Com pins
    SSD1306_Command(0xDB); SSD1306_Command(0x20); // Vcomh
    SSD1306_Command(0x8D); SSD1306_Command(0x14); // Charge pump
    SSD1306_Command(0xAF); // Display ON
}

void SSD1306_Clear(void) {
    for (uint8_t page = 0; page < 8; page++) {
        SSD1306_Command(0xB0 + page);
        SSD1306_Command(0x00);
        SSD1306_Command(0x10);
        for (uint8_t col = 0; col < 128; col++) {
            SSD1306_Data(0x00);
        }
    }
}

void SSD1306_Chessboard(void) {
    for (uint8_t page = 0; page < 8; page++) {
        SSD1306_Command(0xB0 + page);
        SSD1306_Command(0x00);
        SSD1306_Command(0x10);
        for (uint8_t col = 0; col < 128; col++) {
            uint8_t pattern = ((page / 2) + (col / 16)) % 2 ? 0xFF : 0x00;
            SSD1306_Data(pattern);
        }
    }
}