/*
 * main.c
 *
 * Example Coocox project for ST NUCLEO-F401RE boar with STM32F401RE controller.
 *
 * Clock is configured as:
 *   External 8MHz from STLINK V2.
 *   Main clock: 84MHz
 *   AHB clock: 84MHz
 *   APB1 clock: 42MHz
 *   APB2 clcok: 84MHz
 *   Timers clock: 84MHz
 *   SDIO clock: 48MHz
 *
 * Flash Prefetch enabled and 2 wait states (minimum for 84MHz and 3.3V).
 *
 * Project based on a smaller STM32F401RC supported by Coocox. In linker configuration
 * the ram and flash size is updated to suit the STM32F401RE.
 *
 * This example configures the LED pin as output and every second the led toogle.
 * Time base is from TIM3, configured to generate an update interrupt every second.
 *
 * USART2 configured to output from printf function. USART2 is connected to
 * STLINK with 9600 baud. This way one can use the Virtual COM Port from STLINK.
 *
 * A string with a counter is printed in serial every second.
 *
 * Miguel Moreto
 * Florianopolis - Brazil - 2014
 */
#include "stm32f4xx.h"
#include "InitPeriph.h"
#include <stdio.h>

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Flag that is set in interrupt handler. Avoid doing slow stuff in interrupt handlers. */
volatile uint16_t one_second_flag = 0;

int main()
{

    SystemInit();
    SystemCoreClockUpdate();

    /* Configuring Peripherals: */
    MyConfigGPIO();
    MyConfigUSART();
    MyConfigTimers();

    uint16_t counter = 0;

    GPIO_SetBits(LED1_PORT, LED1);

    /* Main loop */
    while(1){

    	if (one_second_flag){
    		/* Toogle LED */
    		GPIO_ToggleBits(LED1_PORT,LED1);
    		/* Write to USART connected with STLINK (USART2) */
    		printf("\r\nTime elapsed is %d seconds.",counter);
    		counter++;
    		/* Reset flag */
    		one_second_flag = 0;
    	} // end one_second_flag
    } // end main loop

} // end main


/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART2, (uint8_t) ch);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
  return ch;
}
