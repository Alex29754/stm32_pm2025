#include "stm32f10x.h"

// простая задержка
void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 8000; i++) {
        __NOP();
    }
}

int main(void) {
    // включаем тактирование портов A и C
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN;

    // PC13 — выход (светодиод)
    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= GPIO_CRH_MODE13_0; // выход push-pull 10 MHz

    // PA0 — вход (кнопка, подтяжка вверх)
    GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
    GPIOA->CRL |= GPIO_CRL_CNF0_1;   // вход с подтяжкой
    GPIOA->ODR |= GPIO_ODR_ODR0;

    uint32_t delay_time = 500;
    uint8_t mode = 0;

    while (1) {
        // Проверяем кнопку
        if (!(GPIOA->IDR & GPIO_IDR_IDR0)) { // если нажата (низкий уровень)
            mode = (mode + 1) % 3;
            if (mode == 0) delay_time = 500;
            if (mode == 1) delay_time = 200;
            if (mode == 2) delay_time = 100;
            delay_ms(300); // антидребезг
        }

        // мигаем LED
        GPIOC->ODR ^= GPIO_ODR_ODR13;
        delay_ms(delay_time);
    }
}
