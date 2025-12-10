#include "stm32f10x.h"

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 8000; i++) {  
        __NOP();
    }
}

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        GPIOC->ODR ^= (1 << 13);  // Переключаем PC13
        TIM2->SR &= ~TIM_SR_UIF;  // Сбрасываем флаг
    }
}

int main(void) {

    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN;
    
    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= GPIO_CRH_MODE13_0;  
    
    // 3. PA0 — вход с подтяжкой вверх (кнопка)
    GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
    GPIOA->CRL |= GPIO_CRL_CNF0_1;    // вход с подтяжкой
    GPIOA->ODR |= (1 << 0);           // подтяжка вверх
    
    // 4. Настраиваем таймер TIM2 для работы на 8 МГц (по умолчанию)
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    
    RCC->APB1RSTR |= RCC_APB1RSTR_TIM2RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM2RST;
 
    TIM2->PSC = 7999;    // Предделитель
    TIM2->ARR = 499;     // Период 500 мс
    
    TIM2->DIER |= TIM_DIER_UIE;      // Прерывание по обновлению
    NVIC_EnableIRQ(TIM2_IRQn);       // Включаем прерывание
    
    TIM2->CR1 |= TIM_CR1_CEN;        // Запускаем таймер
    
    uint8_t button_was_pressed = 0;
    uint8_t speed_mode = 0;
    
    while(1) {
        if(!(GPIOA->IDR & GPIO_IDR_IDR0)) {
            if(!button_was_pressed) {
                button_was_pressed = 1;
                
                TIM2->PSC = TIM2->PSC << 1;  // Умножаем на 2
                TIM2->PSC = TIM2->PSC >> 1;  // Делим на 2
                
                
                speed_mode = (speed_mode + 1) % 3;
                switch(speed_mode) {
                    case 0: TIM2->ARR = 499; break;  // 500 мс
                    case 1: TIM2->ARR = 199; break;  // 200 мс
                    case 2: TIM2->ARR = 99;  break;  // 100 мс
                }
                
                delay_ms(300);  
        } else {
            button_was_pressed = 0;
        }
    }
}