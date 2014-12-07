
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

/* BRTOS Semaphores */
BRTOS_Sem     *SemaphoreKey;

/* BRTOS Mutexes */
BRTOS_Mutex *SerialMutex;

void Task_BlinkLed(void);
void Task_KeyDebounce(void);

