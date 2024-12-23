/*
 * pwm.c
 *
 *  Created on: Nov 18, 2024
 *      Author: Andrej Čilag
 */

#include "pwm.h"
#include "stm32f4xx.h"

static void
tim11_init ();

void
pwm_init ()
{
  tim11_init ();
}

static void
tim11_init ()
{
  // TIM11 -> PB9
  RCC->AHB1ENR |= (0b1 << 1); // Ukljucivanje takta za port B

  const uint8_t PWM_PIN = 9;
  GPIOB->MODER |= (0b10 << PWM_PIN * 2); // Pin je alternativna funkcija

  const uint8_t AF = 3;
  GPIOB->AFR[PWM_PIN / 8] |= (AF << (PWM_PIN * 4) % 32);

  RCC->APB2ENR |= (0b1 << 18); // Ukljucivanje takta za TIM11

  TIM11->CR1 &= ~(0b1 << 2); // Šta može da resetuje (reinicijalizuje) tajmer
  TIM11->CR1 &= ~(0b1 << 1); // Dozvola događaja

  // fclk = 84MHz -> signal: 50Hz
  TIM11->PSC = 84 - 1;
  TIM11->ARR = 20000 - 1;

  TIM11->CCMR1 |= (0b110 << 4); // PWM1 mode - neinvertujemo signal!
  TIM11->CCMR1 |= (0b1 << 3); // Preload

  TIM11->EGR |= (0b1 << 0); // Resetovanje (reinicijalizacija) tajmera

  while (!(TIM11->SR & (0b1 << 0)))
    ; // Čekanje da se izvrši reinicijalizacija tajmera
  TIM11->SR &= ~(0b1 << 0); // Resetovanje statusnog bita

  TIM11->CR1 |= (0b1 << 2); // "Zabrana" resetovanja (reinicijalizacije) tajmera

  TIM11->CCER |= (0b1 << 0); // Ukljucivanje kanala 1 TIM11
  TIM11->CR1 |= (0b1 << 0); // Ukljucivanje TIM11

  // TIM11->CCR1 - registar za podesavanja faktora ispune
}

// Podesavanje faktora ispune za RC servo motor
void
pwm_rc_dc (uint8_t dc)
{
  float y_proc = (dc / 100.0) + 1.0; // Konverzija iz % u (1-2) ms
  float y_dc = 1000.0 * y_proc; // Konverzija iz (1-2) ms u faktor ispune
  TIM11->CCR1 = (uint16_t) y_dc;
}

void
motor1_init ()
{
  // PA10 -> ~M1SLP
  // PA8 -> M1DIR
  // PC7 -> M1PWM

  RCC->AHB1ENR |= ((0b1 << 0) | (0b1 << 2)); // Dozvolaa takta za portove A i C

  const uint8_t M1SLP_PIN = 10; // A
  const uint8_t M1DIR_PIN = 8; // A
  const uint8_t M1PWM_PIN = 7; // C

  GPIOA->MODER |= (0b01 << M1SLP_PIN * 2); // digitalni izlaz
  GPIOA->MODER |= (0b01 << M1DIR_PIN * 2); // digitalni izlaz
  GPIOC->MODER |= (0b10 << M1PWM_PIN * 2); // alternativna funkcija

  const uint8_t AF = 2; // TIM3_CH2
  GPIOC->AFR[M1PWM_PIN / 8] |= (AF << (M1PWM_PIN * 4) % 32);

  // PWM
  RCC->APB1ENR |= (0b1 << 1); // Ukljucivanje takta za TIM3

  TIM3->CR1 &= ~(0b1 << 2); // Šta može da resetuje (reinicijalizuje) tajmer
  TIM3->CR1 &= ~(0b1 << 1); // Dozvola događaja

  // fclk = 84MHz -> signal: 20kHz
  TIM3->PSC = 0;
  TIM3->ARR = 4200 - 1;

  TIM3->CCMR1 |= (0b110 << 12); // PWM1 mode - neinvertujemo signal!
  TIM3->CCMR1 |= (0b1 << 11); // Preload

  TIM3->EGR |= (0b1 << 0); // Resetovanje (reinicijalizacija) tajmera

  while (!(TIM3->SR & (0b1 << 0)))
    ; // Čekanje da se izvrši reinicijalizacija tajmera
  TIM3->SR &= ~(0b1 << 0); // Resetovanje statusnog bita

  TIM3->CR1 |= (0b1 << 2); // "Zabrana" resetovanja (reinicijalizacije) tajmera

  TIM3->CCER |= (0b1 << 4); // Ukljucivanje kanala 2 TIM3
  TIM3->CR1 |= (0b1 << 0); // Ukljucivanje TIM11
}
