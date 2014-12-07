
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

#include "BRTOS.h"

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "InitPeriph.h"


/* This task flashes the LED */
void Task_BlinkLed(void)
{
	/* task setup */
	/* task main loop */
	for (;;){
		GPIO_SetBits(LED1_PORT,LED1);
		DelayTask(50);
		GPIO_ResetBits(LED1_PORT,LED1);
		DelayTask(950);
	}
}





