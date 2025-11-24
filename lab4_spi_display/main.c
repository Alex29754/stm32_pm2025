#include "stm32f10x.h"
#include "spi.h"
#include "ssd1306.h"

void GPIO_Init(void);
void Delay_ms(uint32_t ms);

int main(void) {
    GPIO_Init();
    SPI1_Init();
    SSD1306_Init();
    SSD1306_Clear();
    SSD1306_Chessboard();
    
    while (1) {
        // Мигаем светодиодом на PC13
        GPIOC->ODR ^= GPIO_ODR_ODR13;
        Delay_ms(500);
    }
}

void GPIO_Init(void) {
    // Тактирование портов A и C
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN;
    
    // PA5(SCK), PA7(MOSI) - alternative output
    GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5 | GPIO_CRL_CNF7 | GPIO_CRL_MODE7);
    GPIOA->CRL |= (GPIO_CRL_CNF5_1 | GPIO_CRL_MODE5 | GPIO_CRL_CNF7_1 | GPIO_CRL_MODE7);
    
    // PA4(CS), PA1(DC), PA0(RES) - output
    GPIOA->CRL &= ~(GPIO_CRL_CNF4 | GPIO_CRL_MODE4 | GPIO_CRL_CNF1 | GPIO_CRL_MODE1 | GPIO_CRL_CNF0 | GPIO_CRL_MODE0);
    GPIOA->CRL |= (GPIO_CRL_MODE4 | GPIO_CRL_MODE1 | GPIO_CRL_MODE0);
    
    // PC13(LED) - output
    GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);
    GPIOC->CRH |= GPIO_CRH_MODE13_0;
    
    // Установка начальных состояний
    GPIOA->ODR |= GPIO_ODR_ODR4; // CS = 1
    GPIOC->ODR |= GPIO_ODR_ODR13; // LED off
}

void Delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 1000; i++) {
        __NOP();
    }
}