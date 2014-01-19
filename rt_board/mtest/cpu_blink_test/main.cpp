#include "stm32f0xx.h"

void delay(unsigned a)
{
  volatile unsigned i,j;
  for(i=0 ; i < a ; i++)
    j++;
}


int main(void)
{
  RCC->AHBENR    |= RCC_AHBENR_GPIOBEN;                                 // GPIOB Periph clock enable
  GPIOB->MODER   |= (GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0);        // Configure PB0 and PB1 in output mode
  GPIOB->OTYPER  &= ~(GPIO_OTYPER_OT_0 | ~GPIO_OTYPER_OT_1);            // Ensure push pull mode selected--default
  GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR0|GPIO_OSPEEDER_OSPEEDR1);    // Ensure maximum speed setting (even though it is unnecessary)
  GPIOB->PUPDR   &= ~(GPIO_PUPDR_PUPDR0|GPIO_PUPDR_PUPDR1);             // Ensure all pull up pull down resistors are disabled

  while(true)
  {
    constexpr auto val = 0xFFFF;
    constexpr auto dt  = 100u*1000u;
    GPIOB->BSRR = val;              // set
    delay(20u*dt);
    GPIOB->BRR = val;               // unset
    delay(10u*dt);
  }
}
