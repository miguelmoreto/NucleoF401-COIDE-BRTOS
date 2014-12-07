/*
 * main.c
 *
 * Example Coocox project for BRTOS (Brazilian Real-Time Operating System)
 * using ST NUCLEO-F401RE board with STM32F401RE controller.
 *
 * Clock is configured as:
 * External 8MHz from STLINK V2.
 * Main clock: 84MHz
 * AHB clock: 84MHz
 * APB1 clock: 42MHz
 * APB2 clcok: 84MHz
 * Timers clock: 84MHz
 * SDIO clock: 48MHz
 *
 * Flash Prefetch enabled and 2 wait states (minimum for 84MHz and 3.3V).
 * 
 * This demo uses two tasks. One of them flashes the LED and prints a
 * string with a counter in serial every second.
 * 
 * The second task is used to handle the user button switch press event. When the user press the
 * button a string is printed in serial.
 * 
 * Using a mutex to avoid conflicts with two tasks sharing the serial.
 * 
 * Serial used is USART2 that is connected in STLINK V2-1 and becomes 
 * a Virtual Serial Com port when STLINK is connected.
 *
 * Miguel Moreto
 * Florianopolis - Brazil - 2014
 */
#include "stm32f4xx.h"
#include "InitPeriph.h"

#include "BRTOS.h"
#include "tasks.h"

#include <stdio.h>

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */




int main(void){

    SystemInit();
#if (FPU_SUPPORT != 1)
    /* Disable automatic lazy FPU registers save */
    *(FPU_FPCCR) = 0;
#endif
    SystemCoreClockUpdate();

    /* Configuring Peripherals: */
    MyConfigGPIO();
    MyConfigUSART();
    //MyConfigTimers(); // Not using timer in this demo.

    /* Initialize BRTOS */
    BRTOS_Init();

    GPIO_SetBits(LED1_PORT, LED1);

    if (OSSemCreate(0,&SemaphoreKey) != ALLOC_EVENT_OK){
    	// Oh Oh
    	// It should not enter here!!!
    	while(1){};
    };

    /* Creates a mutex with priority equal to 10.
     * The mutex priority have to be greater than
     * the highest priority of the task the access
     * the shared resource. */
    if (OSMutexCreate(&SerialMutex,10) != ALLOC_EVENT_OK){
    	// Mutex allocation failed
    	// Treat this error here !!!
    }

    /* Install blink led task */
    if(InstallTask(&Task_BlinkLed,"Blink LED",512,1,NULL) != OK){
    	// Oh Oh
    	// It should not enter here!!!
    	while(1){};
    };

    /* Install key debounce task */
    if(InstallTask(&Task_KeyDebounce,"Key debounce",512,2,NULL) != OK){
    	// Oh Oh
    	// It should not enter here!!!
    	while(1){};
    };

    /* Start Task Scheduler */
    if(BRTOSStart() != OK){
    	// Oh Oh
    	// It should not enter here!!!
    	for(;;){};
    };

    return 0;
} // end main


/**
  * @brief  This function handles External lines 15 to 10 interrupt request.
  * @param  None
  * @retval None
  */
void USER_BTN_IRQ_handler(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;

	if(EXTI_GetITStatus(USER_BTN_EXTI_LINE) != RESET)
	{
		/* Disable user button interrupt
		 * it will be re-enable after a perior in
		 * debounce task */
		EXTI_InitStructure.EXTI_Line = USER_BTN_EXTI_LINE;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
		/* Wake up the Key debounce task */
		(void)OSSemPost(SemaphoreKey);
		/* Clear the EXTI line pending bit */
		EXTI_ClearITPendingBit(USER_BTN_EXTI_LINE);
	}
	/* Exit interrupt */
	OS_INT_EXIT_EXT();
}

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
