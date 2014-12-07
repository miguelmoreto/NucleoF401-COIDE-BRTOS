
/*********************************************************************************************************
*                                               BRTOS
*                                Brazilian Real-Time Operating System
*                            Acronymous of Basic Real-Time Operating System
*
*                              
*                                  Open Source RTOS under MIT License
*
*
*
*                                              OS Tasks
*
*
*   Author:   Gustavo Weber Denardin
*   Revision: 1.0
*   Date:     20/03/2009
*
*********************************************************************************************************/
/*
 * Nucleo F401 BRTOS demo
 *
 * Author: Miguel Moreto
 * Florianopolis, Brazil, 2014
 */


/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "BRTOS.h"
#include "tasks.h"
#include "UART.h"

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "InitPeriph.h"


/* This task flashes the LED */
void Task_BlinkLed(void)
{
	/* task setup */
	uint16_t counter = 0;
	/* task main loop */
	for (;;){

		printf("\r\n%d seconds passed.",counter);

		GPIO_SetBits(LED1_PORT,LED1);
		DelayTask(50);
		GPIO_ResetBits(LED1_PORT,LED1);
		counter++;
		DelayTask(950);
	}
}


/* Debounce a Key. This task wakeup from interrupt. */
void Task_KeyDebounce(void){
	/* task setup */
	EXTI_InitTypeDef   EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = USER_BTN_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;

    unsigned char character;
	/* task main loop */

	for (;;){

		/* Wait for key press */
		(void)OSSemPend(SemaphoreKey,0);

		printf("\r\nKey pressed.");
		printf("\r\nType something: ");

		(void)UARTGetChar(USART2,&character, 0);
		printf("\r\nYou typed: %c", character);

		/* If the terminal program sends a string at once,
		 * this loop will get all the remaining values stored
		 * in the Queue. If there is no more data, Queue pend
		 * will wait for 50ms, this time is also used for key
		 * debounce. */
		while (UARTGetChar(USART2,&character, 50) == READ_BUFFER_OK){
			printf("\r\nYou typed: %c", character);
		}
		/* Wait for some milliseconds to debounce key */
		//DelayTask(50);

		/*Re-enable exti line interrupt */
	    EXTI_Init(&EXTI_InitStructure);

	}

}

